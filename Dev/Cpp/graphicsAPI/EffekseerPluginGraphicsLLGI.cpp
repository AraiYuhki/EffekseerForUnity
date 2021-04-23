
#include "../unity/IUnityGraphics.h"
#include "../unity/IUnityInterface.h"
#include "EffekseerPluginGraphicsLLGI.h"
#include <algorithm>
#include <assert.h>

#include "../common/EffekseerPluginMaterial.h"

namespace EffekseerPlugin
{

bool RenderPassLLGI::Initialize(IUnityInterfaces* unityInterface,
								EffekseerRenderer::RendererRef renderer,
								Effekseer::Backend::GraphicsDeviceRef device)
{
	unityInterface_ = unityInterface;
	renderer_ = renderer;
	memoryPool_ = EffekseerRenderer::CreateSingleFrameMemoryPool(device);
	commandList_ = EffekseerRenderer::CreateCommandList(device, memoryPool_);

	return true;
}

class TextureLoaderLLGI : public TextureLoader
{
	std::map<Effekseer::TextureRef, void*> textureData2NativePtr;
	Effekseer::Backend::GraphicsDeviceRef graphicsDevice_;

public:
	TextureLoaderLLGI(TextureLoaderLoad load, TextureLoaderUnload unload, Effekseer::Backend::GraphicsDeviceRef graphicsDevice)
		: TextureLoader(load, unload), graphicsDevice_(graphicsDevice)
	{
	}

	virtual ~TextureLoaderLLGI() override = default;

	virtual Effekseer::TextureRef Load(const EFK_CHAR* path, Effekseer::TextureType textureType)
	{
		// Load from unity
		int32_t width, height, format;
		void* texturePtr = load((const char16_t*)path, &width, &height, &format);
		if (texturePtr == nullptr)
		{
			return nullptr;
		}

		// Convert
		ID3D12Resource* resource = reinterpret_cast<ID3D12Resource*>(texturePtr);
		auto backend = EffekseerRendererDX12::CreateTexture(graphicsDevice_, resource);

		auto textureDataPtr = Effekseer::MakeRefPtr<Effekseer::Texture>();
		textureDataPtr->SetBackend(backend);

		textureData2NativePtr[textureDataPtr] = texturePtr;

		return textureDataPtr;
	}

	virtual void Unload(Effekseer::TextureRef source)
	{
		if (source == nullptr)
		{
			return;
		}

		unload(source->GetPath().c_str(), textureData2NativePtr[source]);
		textureData2NativePtr.erase(source);
	}
};

void GraphicsLLGI::AfterReset(IUnityInterfaces* unityInterface) {}

void GraphicsLLGI::Shutdown(IUnityInterfaces* unityInterface)
{
	MaterialEvent::Terminate();
	graphicsDevice_.Reset();
	renderer_ = nullptr;
}

Effekseer::TextureLoaderRef GraphicsLLGI::Create(TextureLoaderLoad load, TextureLoaderUnload unload)
{
	return Effekseer::MakeRefPtr<TextureLoaderLLGI>(load, unload, graphicsDevice_);
}

Effekseer::ModelLoaderRef GraphicsLLGI::Create(ModelLoaderLoad load, ModelLoaderUnload unload)
{
	if (renderer_ == nullptr)
		return nullptr;

	auto loader = Effekseer::MakeRefPtr<ModelLoader>(load, unload);
	auto internalLoader = EffekseerRenderer::CreateModelLoader(renderer_->GetGraphicsDevice(), loader->GetFileInterface());
	loader->SetInternalLoader(internalLoader);
	return loader;
}

Effekseer::MaterialLoaderRef GraphicsLLGI::Create(MaterialLoaderLoad load, MaterialLoaderUnload unload)
{
	if (renderer_ == nullptr)
		return nullptr;

	auto loader = Effekseer::MakeRefPtr<MaterialLoader>(load, unload);
	auto internalLoader = renderer_->CreateMaterialLoader();
	auto holder = std::make_shared<MaterialLoaderHolder>(internalLoader);
	loader->SetInternalLoader(holder);
	return loader;
}

void GraphicsLLGI::ShiftViewportForStereoSinglePass(bool isShift) {}

void GraphicsLLGI::SetRenderPath(EffekseerRenderer::Renderer* renderer, RenderPass* renderPath)
{
	if (renderPath != nullptr)
	{
		auto rt = static_cast<RenderPassLLGI*>(renderPath);
		renderer_->SetCommandList(rt->GetCommandList());
	}
	else
	{
		renderer_->SetCommandList(nullptr);
	}
}

void GraphicsLLGI::WaitFinish()
{
	if (renderer_ == nullptr)
	{
		return;
	}

	EffekseerRenderer::FlushAndWait(renderer_->GetGraphicsDevice());
}

} // namespace EffekseerPlugin