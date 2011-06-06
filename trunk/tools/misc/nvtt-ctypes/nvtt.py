import ctypes

_nvtt = ctypes.CDLL("libnvtt.dylib")

##

class NvttInputOptions(ctypes.Structure):
  pass

class NvttCompressionOptions(ctypes.Structure):
  pass

class NvttCompressor(ctypes.Structure):
  pass

class NvttOutputOptions(ctypes.Structure):
  pass

##

NvttAlphaMode = ctypes.c_int
NvttBoolean = ctypes.c_int
NvttFormat = ctypes.c_int
NvttInputFormat = ctypes.c_int
NvttMipmapFilter = ctypes.c_int
NvttRoundMode = ctypes.c_int
NvttTextureType = ctypes.c_int
NvttQuality = ctypes.c_int
NvttWrapMode = ctypes.c_int

## InputOptions class

#NVTT_API NvttInputOptions * nvttCreateInputOptions();
nvttCreateInputOptions = _nvtt.nvttCreateInputOptions
nvttCreateInputOptions.restype = ctypes.POINTER(NvttInputOptions)

#NVTT_API void nvttDestroyInputOptions(NvttInputOptions * inputOptions);
nvttDestroyInputOptions = _nvtt.nvttDestroyInputOptions
nvttDestroyInputOptions.restype = None
nvttDestroyInputOptions.argtypes = [ ctypes.POINTER(NvttInputOptions) ]

#NVTT_API void nvttSetInputOptionsTextureLayout(NvttInputOptions * inputOptions, NvttTextureType type, int w, int h, int d);
nvttSetInputOptionsTextureLayout = _nvtt.nvttSetInputOptionsTextureLayout
nvttSetInputOptionsTextureLayout.restype = None
nvttSetInputOptionsTextureLayout.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttTextureType, ctypes.c_int, ctypes.c_int, ctypes.c_int ]

#NVTT_API void nvttResetInputOptionsTextureLayout(NvttInputOptions * inputOptions);
nvttResetInputOptionsTextureLayout = _nvtt.nvttResetInputOptionsTextureLayout
nvttResetInputOptionsTextureLayout.restype = None
nvttResetInputOptionsTextureLayout.argtypes = [ ctypes.POINTER(NvttInputOptions) ]

#NVTT_API NvttBoolean nvttSetInputOptionsMipmapData(NvttInputOptions * inputOptions, const void * data, int w, int h, int d, int face, int mipmap);
nvttSetInputOptionsMipmapData = _nvtt.nvttSetInputOptionsMipmapData
nvttSetInputOptionsMipmapData.restype = NvttBoolean
nvttSetInputOptionsMipmapData.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.POINTER(ctypes.c_void_p), ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int ]

#NVTT_API void nvttSetInputOptionsFormat(NvttInputOptions * inputOptions, NvttInputFormat format);
nvttSetInputOptionsFormat = _nvtt.nvttSetInputOptionsFormat
nvttSetInputOptionsFormat.restype = None
nvttSetInputOptionsFormat.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttInputFormat ]

#NVTT_API void nvttSetInputOptionsAlphaMode(NvttInputOptions * inputOptions, NvttAlphaMode alphaMode);
nvttSetInputOptionsAlphaMode = _nvtt.nvttSetInputOptionsAlphaMode
nvttSetInputOptionsAlphaMode.restype = None
nvttSetInputOptionsAlphaMode.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttAlphaMode ]

#NVTT_API void nvttSetInputOptionsGamma(NvttInputOptions * inputOptions, float inputGamma, float outputGamma);
nvttSetInputOptionsGamma = _nvtt.nvttSetInputOptionsGamma
nvttSetInputOptionsGamma.restype = None
nvttSetInputOptionsGamma.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.c_float, ctypes.c_float ]

#NVTT_API void nvttSetInputOptionsWrapMode(NvttInputOptions * inputOptions, NvttWrapMode mode);
nvttSetInputOptionsWrapMode = _nvtt.nvttSetInputOptionsWrapMode
nvttSetInputOptionsWrapMode.restype = None
nvttSetInputOptionsWrapMode.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttWrapMode ]

#NVTT_API void nvttSetInputOptionsMipmapFilter(NvttInputOptions * inputOptions, NvttMipmapFilter filter);
nvttSetInputOptionsMipmapFilter = _nvtt.nvttSetInputOptionsMipmapFilter
nvttSetInputOptionsMipmapFilter.restype = None
nvttSetInputOptionsMipmapFilter.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttMipmapFilter ]

#NVTT_API void nvttSetInputOptionsMipmapGeneration(NvttInputOptions * inputOptions, NvttBoolean enabled, int maxLevel);
nvttSetInputOptionsMipmapGeneration = _nvtt.nvttSetInputOptionsMipmapGeneration
nvttSetInputOptionsMipmapGeneration.restype = None
nvttSetInputOptionsMipmapGeneration.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttBoolean, ctypes.c_int ]

#NVTT_API void nvttSetInputOptionsKaiserParameters(NvttInputOptions * inputOptions, float width, float alpha, float stretch);
nvttSetInputOptionsKaiserParameters = _nvtt.nvttSetInputOptionsKaiserParameters
nvttSetInputOptionsKaiserParameters.restype = None
nvttSetInputOptionsKaiserParameters.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.c_float, ctypes.c_float, ctypes.c_float ]

#NVTT_API void nvttSetInputOptionsNormalMap(NvttInputOptions * inputOptions, NvttBoolean b);
nvttSetInputOptionsNormalMap = _nvtt.nvttSetInputOptionsNormalMap
nvttSetInputOptionsNormalMap.restype = None
nvttSetInputOptionsNormalMap.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttBoolean ]

#NVTT_API void nvttSetInputOptionsConvertToNormalMap(NvttInputOptions * inputOptions, NvttBoolean b);
nvttSetInputOptionsConvertToNormalMap = _nvtt.nvttSetInputOptionsConvertToNormalMap
nvttSetInputOptionsConvertToNormalMap.restype = None
nvttSetInputOptionsConvertToNormalMap.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttBoolean ]

#NVTT_API void nvttSetInputOptionsHeightEvaluation(NvttInputOptions * inputOptions, float redScale, float greenScale, float blueScale, float alphaScale);
nvttSetInputOptionsHeightEvaluation = _nvtt.nvttSetInputOptionsHeightEvaluation
nvttSetInputOptionsHeightEvaluation.restype = None
nvttSetInputOptionsHeightEvaluation.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float ]

#NVTT_API void nvttSetInputOptionsNormalFilter(NvttInputOptions * inputOptions, float sm, float medium, float big, float large);
nvttSetInputOptionsNormalFilter = _nvtt.nvttSetInputOptionsNormalFilter
nvttSetInputOptionsNormalFilter.restype = None
nvttSetInputOptionsNormalFilter.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float ]

#NVTT_API void nvttSetInputOptionsNormalizeMipmaps(NvttInputOptions * inputOptions, NvttBoolean b);
nvttSetInputOptionsNormalizeMipmaps = _nvtt.nvttSetInputOptionsNormalizeMipmaps
nvttSetInputOptionsNormalizeMipmaps.restype = None
nvttSetInputOptionsNormalizeMipmaps.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttBoolean ]

#NVTT_API void nvttSetInputOptionsMaxExtents(NvttInputOptions * inputOptions, int dim);
nvttSetInputOptionsMaxExtents = _nvtt.nvttSetInputOptionsMaxExtents
nvttSetInputOptionsMaxExtents.restype = None
nvttSetInputOptionsMaxExtents.argtypes = [ ctypes.POINTER(NvttInputOptions), ctypes.c_int ]

#NVTT_API void nvttSetInputOptionsRoundMode(NvttInputOptions * inputOptions, NvttRoundMode mode);
nvttSetInputOptionsRoundMode = _nvtt.nvttSetInputOptionsRoundMode
nvttSetInputOptionsRoundMode.restype = None
nvttSetInputOptionsRoundMode.argtypes = [ ctypes.POINTER(NvttInputOptions), NvttRoundMode ]

## CompressionOptions class.

#NVTT_API NvttCompressionOptions * nvttCreateCompressionOptions();
nvttCreateCompressionOptions = _nvtt.nvttCreateCompressionOptions
nvttCreateCompressionOptions.restype = ctypes.POINTER(NvttCompressionOptions)

#NVTT_API void nvttDestroyCompressionOptions(NvttCompressionOptions * compressionOptions);
nvttDestroyCompressionOptions = _nvtt.nvttDestroyCompressionOptions
nvttDestroyCompressionOptions.restype = None
nvttDestroyCompressionOptions.argtypes = [ ctypes.POINTER(NvttCompressionOptions) ]

#NVTT_API void nvttSetCompressionOptionsFormat(NvttCompressionOptions * compressionOptions, NvttFormat format);
nvttSetCompressionOptionsFormat = _nvtt.nvttSetCompressionOptionsFormat
nvttSetCompressionOptionsFormat.restype = None
nvttSetCompressionOptionsFormat.argtypes = [ ctypes.POINTER(NvttCompressionOptions), NvttFormat ]

#NVTT_API void nvttSetCompressionOptionsQuality(NvttCompressionOptions * compressionOptions, NvttQuality quality);
nvttSetCompressionOptionsQuality = _nvtt.nvttSetCompressionOptionsQuality
nvttSetCompressionOptionsQuality.restype = None
nvttSetCompressionOptionsQuality.argtypes = [ ctypes.POINTER(NvttCompressionOptions), NvttQuality ]

#NVTT_API void nvttSetCompressionOptionsColorWeights(NvttCompressionOptions * compressionOptions, float red, float green, float blue, float alpha);
nvttSetCompressionOptionsColorWeights = _nvtt.nvttSetCompressionOptionsColorWeights
nvttSetCompressionOptionsColorWeights.restype = None
nvttSetCompressionOptionsColorWeights.argtypes = [ ctypes.POINTER(NvttCompressionOptions), ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float ]

#NVTT_API void nvttSetCompressionOptionsPixelFormat(NvttCompressionOptions * compressionOptions, unsigned int bitcount, unsigned int rmask, unsigned int gmask, unsigned int bmask, unsigned int amask);
nvttSetCompressionOptionsPixelFormat = _nvtt.nvttSetCompressionOptionsPixelFormat
nvttSetCompressionOptionsPixelFormat.restype = None
nvttSetCompressionOptionsPixelFormat.argtypes = [ ctypes.POINTER(NvttCompressionOptions), ctypes.c_uint, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint ]

#NVTT_API void nvttSetCompressionOptionsQuantization(NvttCompressionOptions * compressionOptions, NvttBoolean colorDithering, NvttBoolean alphaDithering, NvttBoolean binaryAlpha, int alphaThreshold);
nvttSetCompressionOptionsQuantization = _nvtt.nvttSetCompressionOptionsQuantization
nvttSetCompressionOptionsQuantization.restype = None
nvttSetCompressionOptionsQuantization.argtypes = [ ctypes.POINTER(NvttCompressionOptions), NvttBoolean, NvttBoolean, NvttBoolean, NvttBoolean ]

## OutputOptions class.

#NVTT_API NvttOutputOptions * nvttCreateOutputOptions();
nvttCreateOutputOptions = _nvtt.nvttCreateOutputOptions
nvttCreateOutputOptions.restype = ctypes.POINTER(NvttOutputOptions)

#NVTT_API void nvttDestroyOutputOptions(NvttOutputOptions * outputOptions);
nvttDestroyOutputOptions = _nvtt.nvttDestroyOutputOptions
nvttDestroyOutputOptions.restype = None
nvttDestroyOutputOptions.argtypes = [ ctypes.POINTER(NvttOutputOptions) ]

#NVTT_API void nvttSetOutputOptionsFileName(NvttOutputOptions * outputOptions, const char * fileName);
nvttSetOutputOptionsFileName = _nvtt.nvttSetOutputOptionsFileName
nvttSetOutputOptionsFileName.restype = None
nvttSetOutputOptionsFileName.argtypes = [ ctypes.POINTER(NvttOutputOptions), ctypes.c_char_p ]

#NVTT_API void nvttSetOutputOptionsOutputHeader(NvttOutputOptions * outputOptions, NvttBoolean b);
nvttSetOutputOptionsOutputHeader = _nvtt.nvttSetOutputOptionsOutputHeader
nvttSetOutputOptionsOutputHeader.restype = None
nvttSetOutputOptionsOutputHeader.argtypes = [ ctypes.POINTER(NvttOutputOptions), NvttBoolean ]

##NVTT_API void nvttSetOutputOptionsErrorHandler(NvttOutputOptions * outputOptions, nvttErrorHandler errorHandler);
##NVTT_API void nvttSetOutputOptionsOutputHandler(NvttOutputOptions * outputOptions, nvttOutputHandler outputHandler, nvttImageHandler imageHandler);

## Compressor class

#NVTT_API NvttCompressor * nvttCreateCompressor();
nvttCreateCompressor = _nvtt.nvttCreateCompressor
nvttCreateCompressor.restype = ctypes.POINTER(NvttCompressor)

#NVTT_API void nvttDestroyCompressor(NvttCompressor * compressor);
nvttDestroyCompressor = _nvtt.nvttDestroyCompressor
nvttDestroyCompressor.restype = None
nvttDestroyCompressor.argtypes = [ ctypes.POINTER(NvttCompressor) ]

#NVTT_API NvttBoolean nvttCompress(const NvttCompressor * compressor, const NvttInputOptions * inputOptions, const NvttCompressionOptions * compressionOptions, const NvttOutputOptions * outputOptions);
nvttCompress = _nvtt.nvttCompress
nvttCompress.restype = NvttBoolean
nvttCompress.argtypes = [ ctypes.POINTER(NvttCompressor), ctypes.POINTER(NvttInputOptions), ctypes.POINTER(NvttCompressionOptions), ctypes.POINTER(NvttOutputOptions) ]

#NVTT_API int nvttEstimateSize(const NvttCompressor * compressor, const NvttInputOptions * inputOptions, const NvttCompressionOptions * compressionOptions);
nvttEstimateSize = _nvtt.nvttEstimateSize
nvttEstimateSize.restype = ctypes.c_int
nvttEstimateSize.argtypes = [ ctypes.POINTER(NvttCompressor), ctypes.POINTER(NvttInputOptions), ctypes.POINTER(NvttCompressionOptions) ]

