#ifndef ASSIMP_REVISION_H_INC
#define ASSIMP_REVISION_H_INC

// Zylann: I had to do this because otherwise I couldn't integrate Assimp with the premake build system
//#define GitVersion 0x@GIT_COMMIT_HASH@
//#define GitBranch "@GIT_BRANCH@"
#define GitVersion 0
#define GitBranch "@GIT_BRANCH@"

#endif // ASSIMP_REVISION_H_INC
