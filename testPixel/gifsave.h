// Made from gif.h
// gif.h
// by Charlie Tangora
// Public domain.
// Email me : ctangora -at- gmail -dot- com
//
// This file offers a simple, very limited way to create animated GIFs directly in code.
//
// Those looking for particular cleverness are likely to be disappointed; it's pretty
// much a straight-ahead implementation of the GIF format with optional Floyd-Steinberg
// dithering. (It does at least use delta encoding - only the changed portions of each
// frame are saved.)
//
// So resulting files are often quite large. The hope is that it will be handy nonetheless
// as a quick and easily-integrated way for programs to spit out animations.
//
// Only RGBA8 is currently supported as an input format. (The alpha is ignored.)
//
// USAGE:
// Create a GifWriter struct. Pass it to GifBegin() to initialize and write the header.
// Pass subsequent frames to GifWriteFrame().
// Finally, call GifEnd() to close the file handle and free memory.
//


#ifndef GIFSAVE_H
#define GIFSAVE_H

#include <stdio.h>   // for FILE*
#include <string.h>  // for memcpy and bzero
#include <stdint.h>  // for integer typedefs

// Define these macros to hook into a custom memory allocator.
// TEMP_MALLOC and TEMP_FREE will only be called in stack fashion - frees in the reverse order of mallocs
// and any temp memory allocated by a function will be freed before it exits.
// MALLOC and FREE are used only by GifBegin and GifEnd respectively (to allocate a buffer the size of the image, which
// is used to find changed pixels for delta-encoding.)

#ifndef GIF_TEMP_MALLOC
#include <stdlib.h>
#define GIF_TEMP_MALLOC malloc
#endif

#ifndef GIF_TEMP_FREE
#include <stdlib.h>
#define GIF_TEMP_FREE free
#endif

#ifndef GIF_MALLOC
#include <stdlib.h>
#define GIF_MALLOC malloc
#endif

#ifndef GIF_FREE
#include <stdlib.h>
#define GIF_FREE free
#endif


class GifSave
{

    const int kGifTransIndex= 0 ;
public:
    GifSave();
    struct GifPalette
    {
        int bitDepth;

        uint8_t r[256];
        uint8_t g[256];
        uint8_t b[256];

        // k-d tree over RGB space, organized in heap fashion
        // i.e. left child of node i is node i*2, right child is node i*2+1
        // nodes 256-511 are implicitly the leaves, containing a color
        uint8_t treeSplitElt[255];
        uint8_t treeSplit[255];
    };
    int GifIMax(int l, int r);
    int GifIMin(int l, int r);
    int GifIAbs(int i);

    void GifGetClosestPaletteColor(GifPalette* pPal, int r, int g, int b, int& bestInd, int& bestDiff, int treeRoot = 1);
    void GifSwapPixels(uint8_t* image, int pixA, int pixB);
    int GifPartition(uint8_t* image, const int left, const int right, const int elt, int pivotIndex);
    void GifPartitionByMedian(uint8_t* image, int left, int right, int com, int neededCenter);
    void GifSplitPalette(uint8_t* image, int numPixels, int firstElt, int lastElt, int splitElt, int splitDist, int treeNode, bool buildForDither, GifPalette* pal);
    int GifPickChangedPixels( const uint8_t* lastFrame, uint8_t* frame, int numPixels );
    void GifMakePalette( const uint8_t* lastFrame, const uint8_t* nextFrame, uint32_t width, uint32_t height, int bitDepth, bool buildForDither, GifPalette* pPal );
    void GifDitherImage( const uint8_t* lastFrame, const uint8_t* nextFrame, uint8_t* outFrame, uint32_t width, uint32_t height, GifPalette* pPal );
    void GifThresholdImage( const uint8_t* lastFrame, const uint8_t* nextFrame, uint8_t* outFrame, uint32_t width, uint32_t height, GifPalette* pPal );

    struct GifBitStatus
    {
        uint8_t bitIndex;  // how many bits in the partial byte written so far
        uint8_t byte;      // current partial byte

        uint32_t chunkIndex;
        uint8_t chunk[256];   // bytes are written in here until we have 256 of them, then written to the file
    };

    void GifWriteBit( GifBitStatus& stat, uint32_t bit );
    void GifWriteChunk( FILE* f, GifBitStatus& stat );
    void GifWriteCode( FILE* f, GifBitStatus& stat, uint32_t code, uint32_t length );

    struct GifLzwNode
    {
        uint16_t m_next[256];
    };

    void GifWritePalette( const GifPalette* pPal, FILE* f );

    void GifWriteLzwImage(FILE* f, uint8_t* image, uint32_t left, uint32_t top,  uint32_t width, uint32_t height, uint32_t delay, GifPalette* pPal);

    struct GifWriter
    {
        FILE* f;
        uint8_t* oldImage;
        bool firstFrame;
    };

    GifWriter storage;
    bool GifBegin( GifWriter* writer, const char* filename, uint32_t width, uint32_t height, uint32_t delay, int32_t bitDepth = 8, bool dither = false );

    bool GifWriteFrame( GifWriter* writer, const uint8_t* image, uint32_t width, uint32_t height, uint32_t delay, int bitDepth = 8, bool dither = false );

    bool GifEnd( GifWriter* writer );


};

#endif // GIFSAVE_H
