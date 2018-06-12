#pragma once

#include "Engine/RHI/RHITypes.hpp"

#include "Engine/Core/Vertex3D.hpp"

#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Matrix4.hpp"

#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/StructuredBuffer.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"

#include <filesystem>
#include <map>
#include <string>

class BlendState;
class Camera3D;
class ConstantBuffer;
class DepthStencilState;
class IndexBuffer;
class IntVector3;
class KerningFont;
class Material;
class RasterState;
struct RasterDesc;
class Rgba;
class RHIDeviceContext;
class RHIDevice;
class RHIOutput;
class RHIInstance;
class Sampler;
struct SamplerDesc;
class Shader;
class ShaderProgram;
class SpriteSheet;
class StructuredBuffer;
class Texture;
class Texture1D;
class Texture2D;
class Texture3D;
class VertexBuffer;

struct depthstencil_state_t {
    bool depth_enabled;
    bool depth_write;
    bool stencil_enabled;
    bool stencil_read;
    bool stencil_write;
    StencilOperation stencil_failFrontOp;
    StencilOperation stencil_failBackOp;
    StencilOperation stencil_failDepthFrontOp;
    StencilOperation stencil_failDepthBackOp;
    StencilOperation stencil_passFrontOp;
    StencilOperation stencil_passBackOp;
    ComparisonFunction stencil_testFront;
    ComparisonFunction stencil_testBack;
};

struct matrix_buffer_t {
    Matrix4 model = {};
    Matrix4 view = {};
    Matrix4 projection = {};
};

struct time_buffer_t {
    float game_time = 0.0f;
    float system_time = 0.0f;
    float game_frame_time = 0.0f;
    float system_frame_time = 0.0f;
};

class Renderer {
public:
    Renderer() = default;
    Renderer(unsigned int width, unsigned int height);
    ~Renderer();

    void Initialize();
    void BeginFrame();
    void Update(float deltaSeconds);
    void Render() const;
    void EndFrame();

    Texture* CreateOrGetTexture(const std::string& filepath, const IntVector3& dimensions);

    bool RegisterTexture(const std::string& name, Texture* texture);
    void SetTexture(Texture* texture, unsigned int registerIndex = 0);

    Texture* GetTexture(const std::string& nameOrFile);

    Texture* CreateDepthStencil(RHIDevice* owner, const IntVector2& dimensions);

    Texture* Create1DTexture(const std::string& filepath, const BufferUsage& bufferUsage, const BufferBindUsage& bindUsage, const ImageFormat& imageFormat);
    Texture* Create1DTextureFromMemory(const unsigned char* data, unsigned int width = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create1DTextureFromMemory(const std::vector<Rgba>& data, unsigned int width = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create2DTexture(const std::string& filepath, const BufferUsage& bufferUsage, const BufferBindUsage& bindUsage, const ImageFormat& imageFormat);
    Texture* Create2DTextureFromMemory(const unsigned char* data, unsigned int width = 1, unsigned int height = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create2DTextureFromMemory(const std::vector<Rgba>& data, unsigned int width = 1, unsigned int height = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create2DTextureArrayFromMemory(const unsigned char* data, unsigned int width = 1, unsigned int height = 1, unsigned int depth = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create3DTexture(const std::string& filepath, const IntVector3& dimensions, const BufferUsage& bufferUsage, const BufferBindUsage& bindUsage, const ImageFormat& imageFormat);
    Texture* Create3DTextureFromMemory(const unsigned char* data, unsigned int width = 1, unsigned int height = 1, unsigned int depth = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* Create3DTextureFromMemory(const std::vector<Rgba>& data, unsigned int width = 1, unsigned int height = 1, unsigned int depth = 1, const BufferUsage& bufferUsage = BufferUsage::STATIC, const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE, const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);
    Texture* CreateTexture(const std::string& filepath
                           , const IntVector3& dimensions
                           , const BufferUsage& bufferUsage = BufferUsage::STATIC
                           , const BufferBindUsage& bindUsage = BufferBindUsage::SHADER_RESOURCE
                           , const ImageFormat& imageFormat = ImageFormat::R8G8B8A8_UNORM);

    SpriteSheet* CreateSpriteSheet(const std::string& filepath);

    void SetRenderTarget(Texture* color_target = nullptr, Texture* depthstencil_target = nullptr);
    void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    void SetViewportAsPercent(float x, float y, float w, float h);
    void ClearColor(const Rgba& color);
    void ClearTargetColor(Texture* target, const Rgba& color);
    void ClearDepthStencilBuffer();
    void Present();

    void DrawPoint(const Vertex3D& point);
    void DrawPoint(const Vector3& point, const Rgba& color = Rgba::WHITE, const Vector2& tex_coords = Vector2::ZERO);
    void Draw(const PrimitiveType& topology, const std::vector<Vertex3D>& vbo);
    void Draw(const PrimitiveType& topology, const std::vector<Vertex3D>& vbo, std::size_t vertex_count);
    void DrawIndexed(const PrimitiveType& topology, const std::vector<Vertex3D>& vbo, const std::vector<unsigned int>& ibo);
    void DrawIndexed(const PrimitiveType& topology, const std::vector<Vertex3D>& vbo, const std::vector<unsigned int>& ibo, std::size_t vertex_count);

    RHIDeviceContext* GetDeviceContext() const;
    RHIDevice* GetDevice() const;
    RHIOutput* GetOutput() const;

    ShaderProgram* GetShaderProgram(const std::string& nameOrFile);
    ShaderProgram* CreateShaderProgramFromHlslFile(const std::string& filepath, const std::string& entryPoint, InputLayout* inputLayout, const PipelineStage& target);

    void CreateAndRegisterRasterStateFromRasterDescription(const std::string& name, const RasterDesc& desc);
    void SetRasterState(RasterState* raster);
    RasterState* GetRasterState(const std::string& name);

    void CreateAndRegisterSamplerFromSamplerDescription(const std::string& name, const SamplerDesc& desc);
    Sampler* GetSampler(const std::string& name);
    void SetSampler(Sampler* sampler);

    void SetVSync(bool value);

    Material* CreateMaterialFromFont(KerningFont* font);
    bool RegisterMaterial(const std::string& filepath);
    void RegisterMaterial(Material* mat);
    void RegisterMaterialsFromFolder(const std::string& folderpath, bool recursive = false);
    std::size_t GetMaterialCount();
    Material* GetMaterial(const std::string& nameOrFile);
    void SetMaterial(Material* material);
    const std::map<std::string, Texture*>& GetLoadedTextures() const;

    std::size_t GetShaderCount() const;
    Shader* GetShader(const std::string& nameOrFile);

    std::size_t GetFontCount() const;
    KerningFont* GetFont(const std::string& nameOrFile);
    bool RegisterFont(const std::string& filepath);
    void RegisterFont(KerningFont* font);
    void RegisterFontsFromFolder(const std::string& folderpath, bool recursive = false);

    void SetModelMatrix(const Matrix4& mat);
    void SetViewMatrix(const Matrix4& mat);
    void SetProjectionMatrix(const Matrix4& mat);
    void SetOrthoProjection(const Vector2& leftBottom, const Vector2& rightTop, const Vector2& near_far);
    void SetOrthoProjection(const Vector2& dimensions, const Vector2& origin, float nearz, float farz);
    void SetOrthoProjectionFromViewHeight(float viewHeight, float aspectRatio, float nearz, float farz);
    void SetOrthoProjectionFromViewWidth(float viewWidth, float aspectRatio, float nearz, float farz);
    void SetOrthoProjectionFromCamera(const Camera3D& camera);
    void SetPerspectiveProjection(const Vector2& vfovDegrees_aspect, const Vector2& nz_fz);
    void SetPerspectiveProjectionFromCamera(const Camera3D& camera);

    void AppendModelMatrix(const Matrix4& modelMatrix);

    void SetConstantBuffer(unsigned int index, ConstantBuffer* buffer);
    void SetStructuredBuffer(unsigned int index, StructuredBuffer* buffer);

    void DrawPoint2D(float pointX, float pointY, const Rgba& color = Rgba::WHITE);
    void DrawPoint2D(const Vector2& point, const Rgba& color = Rgba::WHITE);
    void DrawLine2D(float startX, float startY, float endX, float endY, const Rgba& color = Rgba::WHITE, float thickness = 0.0f);
    void DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& color = Rgba::WHITE, float thickness = 0.0f);
    void DrawQuad2D(float left, float bottom, float right, float top, const Rgba& color = Rgba::WHITE);
    void DrawQuad2D(const Vector2& position = Vector2::ZERO, const Vector2& halfExtents = Vector2(0.5f, 0.5f), const Rgba& color = Rgba::WHITE);
    void DrawQuad2D(const Rgba& color);
    void DrawCircle2D(float centerX, float centerY, float radius, const Rgba& color = Rgba::WHITE);
    void DrawCircle2D(const Vector2& center, float radius, const Rgba& color = Rgba::WHITE);

    void DrawPolygon2D(float centerX, float centerY, float radius, std::size_t numSides = 3, const Rgba& color = Rgba::WHITE);
    void DrawPolygon2D(const Vector2& center, float radius, std::size_t numSides = 3, const Rgba& color = Rgba::WHITE);

    void DrawTextLine(KerningFont* font, const std::string& text, const Rgba& color = Rgba::WHITE, float scale = 1.0f);

protected:
private:

    void RegisterShaderProgram(const std::string& name, ShaderProgram * sp);
    void RegisterShader(const std::string& name, Shader* shader);
    void RegisterMaterial(const std::string& name, Material* mat);
    bool RegisterMaterial(const std::experimental::filesystem::path& filepath);
    void RegisterMaterialsFromFolder(const std::experimental::filesystem::path& folderpath, bool recursive = false);
    void RegisterRasterState(const std::string& name, RasterState* raster);
    void RegisterSampler(const std::string& name, Sampler* sampler);
    void RegisterFont(const std::string& name, KerningFont* font);
    bool RegisterFont(const std::experimental::filesystem::path& filepath);
    void RegisterFontsFromFolder(const std::experimental::filesystem::path& folderpath, bool recursive = false);

    void UpdateVbo(const VertexBuffer::buffer_t& vbo);
    void UpdateIbo(const IndexBuffer::buffer_t& ibo);

    void Draw(const PrimitiveType& topology, VertexBuffer* vbo, std::size_t vertex_count);
    void DrawIndexed(const PrimitiveType& topology, VertexBuffer* vbo, IndexBuffer* ibo, std::size_t index_count);

    void CreateAndRegisterDefaultTextures();
    Texture* CreateDefaultTexture();
    Texture* CreateInvalidTexture();
    Texture* CreateDefaultDiffuseTexture();
    Texture* CreateDefaultNormalTexture();
    Texture* CreateDefaultLightingTexture();
    Texture* CreateDefaultSpecularTexture();
    Texture* CreateDefaultOcclusionTexture();
    Texture* CreateDefaultEmissiveTexture();

    void CreateAndRegisterDefaultShaderPrograms();
    ShaderProgram* CreateDefaultShaderProgram();
    ShaderProgram* CreateDefaultUnlitShaderProgram();

    void CreateAndRegisterDefaultShaders();
    Shader* CreateDefaultShader();
    Shader* CreateDefaultUnlitShader();
    Shader* CreateDefault2DShader();

    void CreateAndRegisterDefaultMaterials();
    Material* CreateDefaultMaterial();
    Material* CreateDefaultUnlitMaterial();

    void CreateAndRegisterDefaultSamplers();
    Sampler* CreateDefaultSampler();

    void CreateAndRegisterDefaultRasterStates();
    RasterState* CreateWireframeRaster();
    RasterState* CreateSolidRaster();
    RasterState* CreateWireframeNoCullingRaster();
    RasterState* CreateSolidNoCullingRaster();
    RasterState* CreateWireframeFrontCullingRaster();
    RasterState* CreateSolidFrontCullingRaster();

    void UnbindAllShaderResources();

    SpriteSheet* CreateSpriteSheetFromGif(const std::string& filepath);

    matrix_buffer_t _matrix_data = {};
    time_buffer_t _time_data = {};
    std::size_t _current_vbo_size = 0;
    std::size_t _current_ibo_size = 0;
    constexpr static unsigned int MATRIX_BUFFER_INDEX = 0;
    RHIDeviceContext* _rhi_context = nullptr;
    RHIDevice* _rhi_device = nullptr;
    RHIOutput* _rhi_output = nullptr;
    RHIInstance* _rhi_instance = nullptr;
    Texture* _current_target = nullptr;
    Texture* _current_depthstencil = nullptr;
    Texture* _default_depthstencil = nullptr;
    DepthStencilState* _current_depthstencil_state = nullptr;
    RasterState* _current_raster_state = nullptr;
    Sampler* _current_sampler = nullptr;
    Material* _current_material = nullptr;
    IntVector2 _window_dimensions = IntVector2::ZERO;
    RHIOutputMode _current_outputMode = RHIOutputMode::WINDOWED;
    VertexBuffer* _temp_vbo = nullptr;
    IndexBuffer* _temp_ibo = nullptr;
    ConstantBuffer* _matrix_cb = nullptr;
    ConstantBuffer* _time_cb = nullptr;
    std::map<std::string, Texture*> _textures = {};
    std::map<std::string, ShaderProgram*> _shader_programs = {};
    std::map<std::string, Material*> _materials = {};
    std::map<std::string, Shader*> _shaders = {};
    std::map<std::string, Sampler*> _samplers = {};
    std::map<std::string, RasterState*> _rasters = {};
    std::map<std::string, KerningFont*> _fonts = {};
    bool _vsync = false;

    friend class Shader;
};
