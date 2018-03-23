#include "Engine/Renderer/DirectX/DXTexture1D.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"

#include "Engine/Renderer/DirectX/DX11.hpp"
#include "Engine/Renderer/DirectX/DXDevice.hpp"

DXTexture1D::DXTexture1D(DXDevice* device, ID3D11Texture1D* texture)
    : DXTexture()
    , _dx_resource(nullptr)
{
    SetDeviceAndTexture(device, texture);
}

DXTexture1D::DXTexture1D(DXTexture1D&& r_other) noexcept
    : DXTexture(std::move(r_other))
    , _dx_resource(r_other._dx_resource)
{
    r_other._dx_resource = nullptr;
}

DXTexture1D::~DXTexture1D() {
    _dx_resource->Release();
    _dx_resource = nullptr;
}

void DXTexture1D::SetDeviceAndTexture(DXDevice* device, ID3D11Texture1D* texture) {

    GUARANTEE_OR_DIE(device, "DXTexture1D: Invalid device.");

    _device = dynamic_cast<RHIDevice*>(device);
    _dx_resource = texture;

    D3D11_TEXTURE1D_DESC t_desc;
    std::memset(&t_desc, 0, sizeof(t_desc));
    _dx_resource->GetDesc(&t_desc);
    _dimensions = IntVector3(t_desc.Width, 0, 0);

    bool success = true;
    if(t_desc.BindFlags & D3D11_BIND_RENDER_TARGET) {
        success &= SUCCEEDED(device->GetDxDevice()->CreateRenderTargetView(_dx_resource, nullptr, &_dx_rtv)) == true;
    }

    if(t_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
        success &= SUCCEEDED(device->GetDxDevice()->CreateShaderResourceView(_dx_resource, nullptr, &_dx_srv)) == true;
    }

    if(t_desc.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
        success &= SUCCEEDED(device->GetDxDevice()->CreateDepthStencilView(_dx_resource, nullptr, &_dx_dsv));
    }

    if(t_desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
        D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;
        desc.Format = t_desc.Format;

        success &= SUCCEEDED(device->GetDxDevice()->CreateUnorderedAccessView(_dx_resource, &desc, &_dx_uav));
    }
    if(!success) {
        ERROR_AND_DIE("Set device and texture failed.");
    }
}

bool DXTexture1D::IsValid() const noexcept {
    return _dx_resource != nullptr;
}

void DXTexture1D::SetDebugName([[maybe_unused]] char const* name) noexcept {
#if _DEBUG
    if((_dx_resource != nullptr) && (name != nullptr)) {
        _dx_resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(name) + 1, name);
    }
#endif
}

DXTexture1D& DXTexture1D::operator=(DXTexture1D&& r_rhs) noexcept {
    DXTexture::operator=(std::move(r_rhs));
    _dx_resource = r_rhs._dx_resource;
    r_rhs._dx_resource = nullptr;
    return *this;
}

