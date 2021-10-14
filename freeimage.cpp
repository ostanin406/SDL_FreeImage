#include <FreeImage.h>
#include <SDL.h>

#define IsSupportReading(a) ((a != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(a))
#define IsSupportWriting(a) ((a != FIF_UNKNOWN) && FreeImage_FIFSupportsWriting(a))

#pragma warning(push)
#pragma warning(disable:26812)
FREE_IMAGE_FORMAT CreateFileType(const char* file)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(file);
    return (fif == FIF_UNKNOWN) ? FreeImage_GetFIFFromFilename(file) : fif;
}

FIBITMAP* LoadImage(const char* file)
{
	FREE_IMAGE_FORMAT fif = CreateFileType(file);
    return (IsSupportReading(fif)) ? FreeImage_Load(fif, file) : NULL;
}

BOOL SaveImage(const char* file, FIBITMAP* dib)
{
    FREE_IMAGE_FORMAT fif = CreateFileType(file);
    return (IsSupportWriting(fif)) ? FreeImage_Save(fif, dib, file) : NULL;
}
#pragma warning(pop)

SDL_Surface* SDL_LoadIMG(const char* file)
{
	FIBITMAP* dib = LoadImage(file);
	if (!dib) return NULL;

	FreeImage_FlipVertical(dib);

    return SDL_CreateRGBSurfaceFrom(
        FreeImage_GetBits(dib),
        FreeImage_GetWidth(dib),
        FreeImage_GetHeight(dib),
        FreeImage_GetBPP(dib),
        FreeImage_GetPitch(dib),
        FreeImage_GetRedMask(dib),
        FreeImage_GetGreenMask(dib),
        FreeImage_GetBlueMask(dib),
        FreeImage_IsTransparent(dib) ? 0xFF000000 : 0);
}

int SDL_SaveIMG(const char* file, SDL_Surface* surface)
{
    FIBITMAP* dib = FreeImage_ConvertFromRawBits(
        (BYTE*)surface->pixels,
        surface->w, surface->h,
        surface->pitch,
        surface->format->BitsPerPixel,
        surface->format->Rmask,
        surface->format->Gmask,
        surface->format->Bmask,
        true);

    if (!dib) return -1;

    //FreeImage_FlipVertical(dib);
    return SaveImage(file, dib) ? 0 : -1;
}