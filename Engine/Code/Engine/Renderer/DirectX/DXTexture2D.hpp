#pragma once

#include "Engine/Renderer/DirectX/DXTexture.hpp"

class DXDevice;

class DXTexture2D : public DXTexture {
public:
    DXTexture2D() = default;
    DXTexture2D(const DXTexture2D& other) noexcept = delete;
    DXTexture2D(DXTexture2D&& r_other) noexcept;
    DXTexture2D& operator=(const DXTexture2D& rhs) noexcept = delete;
    DXTexture2D& operator=(DXTexture2D&& r_rhs) noexcept;
    virtual ~DXTexture2D();
    
    void SetDeviceAndTexture(DXDevice* device, ID3D11Texture2D* texture);

protected:
private:
    ID3D11Texture2D* _dx_resource = nullptr;
};