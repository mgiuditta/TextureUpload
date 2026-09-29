project(TextureUpload)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# xbyak only provides x86 CPU detection (MultiISA.cpp); ARM64 uses a fixed feature set.
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)$")
	set(TEXTURE_UPLOAD_ARM64 ON)
endif()

if(NOT TEXTURE_UPLOAD_ARM64)
	add_subdirectory("ext/xbyak")
endif()

set (SOURCES
	"src/common/Pcsx2Types.h"
	"src/common/Pcsx2Defs.h"
	"src/common/VectorIntrin.h"
	"src/GSBlock.cpp"
	"src/GSBlock.h"
	"src/GSTables.cpp"
	"src/GSTables.h"
	"src/GSRegs.h"
	"src/GSExtra.h"
	"src/GSLocalMemoryMultiISA.cpp"
	"src/GSAlignedClass.h"
	"src/GSClut.cpp"
	"src/GSClut.h"
	"src/GSLocalMemory.cpp"
	"src/GSLocalMemory.h"
	"src/GS.h"
	"src/MultiISA.cpp"
	"src/MultiISA.h"
	"src/GSXXH.h"
	"src/TextureUpload.cpp"
	"src/TextureUpload.h"
	"src/GSVector.h"
	"src/GSVector4.h"
	"src/GSVector4i.h"
	"src/GSVector8.h"
	"src/GSVector8i.h"
	"src/UploadBuffer.cpp"
	"include/UploadBuffer.h"
)

add_library(TextureUpload ${SOURCES})

if(NOT TEXTURE_UPLOAD_ARM64)
	target_link_libraries(TextureUpload xbyak)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	if(NOT TEXTURE_UPLOAD_ARM64)
		target_compile_options(TextureUpload PUBLIC "-msse4.1")
	endif()
	# macOS has shm_open in libSystem and no librt.
	if(NOT WIN32 AND NOT APPLE)
		target_link_libraries(TextureUpload rt)
	endif()
endif()

target_compile_definitions(TextureUpload PUBLIC "NOMINMAX")

target_precompile_headers(TextureUpload PRIVATE "src/common/Pcsx2Types.h")

target_include_directories(TextureUpload PRIVATE "ext/xxHash")

target_include_directories(TextureUpload PUBLIC "src")
target_include_directories(TextureUpload PUBLIC "include")