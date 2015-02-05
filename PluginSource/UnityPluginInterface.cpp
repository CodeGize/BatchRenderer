
// --------------------------------------------------------------------------
// Include headers for the graphics APIs we support

#include "UnityPluginInterface.h"
#include "CopyToTexture.h"


CopyToTextureBase *g_c2t = nullptr;


extern "C" void EXPORT_API UnitySetGraphicsDevice(void* device, int deviceType, int eventType)
{
    if (eventType == kGfxDeviceEventInitialize) {
    #if SUPPORT_D3D9
        if (deviceType == kGfxRendererD3D9) {
            // todo
        }
    #endif // SUPPORT_D3D9
    #if SUPPORT_D3D11
        if (deviceType == kGfxRendererD3D11) {
            g_c2t = CreateCopyToTextureD3D11(device);
        }
    #endif // SUPPORT_D3D11
    #if SUPPORT_OPENGL
        if (deviceType == kGfxRendererOpenGL) {
            g_c2t = CreateCopyToTextureOpenGL(device);
        }
    #endif // SUPPORT_OPENGL
    }

    if (eventType == kGfxDeviceEventShutdown) {
        delete g_c2t;
        g_c2t = nullptr;
    }
}

extern "C" void EXPORT_API UnityRenderEvent(int eventID)
{
}

extern "C" void EXPORT_API CopyToTexture(void *texptr, int width, int height, void *data, int data_num, DataConversion c)
{
    g_c2t->copy(texptr, width, height, data, data_num, c);
}



const void* CopyToTextureBase::getDataPointer(const void *data, int data_num, DataConversion conv)
{
    if (conv == Float3ToFloat4) {
        const float3 *src = (const float3*)data;
        m_buffer.resize(data_num);
        for (int i = 0; i < data_num; ++i) {
            m_buffer[i] = (float4&)src[i];
        }
        return &m_buffer[0];
    }
    else {
        return data;
    }
}
