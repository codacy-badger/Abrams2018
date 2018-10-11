#include "Engine/Renderer/Texture.hpp"

#include "Engine/Renderer/DirectX/DX11.hpp"

Texture::Texture(const RHIDevice* device) noexcept
    : _device(device)
{
    /* DO NOTHING */
}

Texture::Texture(Texture&& r_other) noexcept
: _device(r_other._device)
,_dimensions(r_other._dimensions)
,_isLoaded(r_other._isLoaded)
,_isArray(r_other._isArray)
,_dsv(r_other._dsv)
,_rtv(r_other._rtv)
{
    r_other._device = nullptr;
    r_other._dimensions = IntVector3::ZERO;
    r_other._isLoaded = false;
    r_other._isArray = false;
    r_other._dsv = nullptr;
    r_other._rtv = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept {
    _device = rhs._device;
    _dimensions = rhs._dimensions;
    _isLoaded = rhs._isLoaded;
    _isArray = rhs._isArray;
    _dsv = rhs._dsv;
    _rtv = rhs._rtv;

    rhs._device = nullptr;
    rhs._dimensions = IntVector3::ZERO;
    rhs._isLoaded = false;
    rhs._isArray = false;
    rhs._dsv = nullptr;
    rhs._rtv = nullptr;

    return *this;
}

Texture::~Texture() noexcept {
    _device = nullptr;
    if(_dsv) {
        _dsv->Release();
        _dsv = nullptr;
    }
    if(_rtv) {
        _rtv->Release();
        _rtv = nullptr;
    }
    if(_srv) {
        _srv->Release();
        _srv = nullptr;
    }
    if(_uav) {
        _uav->Release();
        _uav = nullptr;
    }
}

const IntVector3& Texture::GetDimensions() const noexcept {
    return _dimensions;
}

void Texture::IsLoaded(bool is_loaded) noexcept {
    _isLoaded = is_loaded;
}

bool Texture::IsLoaded() const noexcept {
    return _isLoaded;
}

bool Texture::IsArray() const {
    return _isArray;
}

ID3D11DepthStencilView* Texture::GetDepthStencilView() {
    return _dsv;
}

ID3D11RenderTargetView* Texture::GetRenderTargetView() {
    return _rtv;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() {
    return _srv;
}
