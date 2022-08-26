#include <wchar.h>
#include <jni.h>
#include <dokan.h>

static JavaVM *jvm;

static jlong filetimeOffset = 116444736000000000;

static jlong filetimeToJLong(FILETIME wt) {
    LONGLONG low = wt.dwLowDateTime;
    LONGLONG high = wt.dwHighDateTime;
    jlong a = high << 32 | low;
    return (a - filetimeOffset) / 10000;
}

static void jlongToFiletime(jlong l, PFILETIME filetime) {
    jlong a = (l * 10000) + filetimeOffset;
    filetime->dwHighDateTime = a >> 32;
    filetime->dwLowDateTime = a & 0xFFFFFFFF;
}

static jobject javaDokanFileInfo(JNIEnv *env, PDOKAN_FILE_INFO DokanFileInfo) {
    jlong context = (jlong) DokanFileInfo->Context;
    jboolean deleteOnClose = DokanFileInfo->DeleteOnClose;
    jlong dokanContext = (jlong) DokanFileInfo->DokanContext;
    jboolean isDirectory = DokanFileInfo->IsDirectory;
    jboolean nocache = DokanFileInfo->Nocache;
    jboolean pagingIo = DokanFileInfo->PagingIo;
    jint processId = (jint) DokanFileInfo->ProcessId;
    jboolean synchronousIo = DokanFileInfo->SynchronousIo;
    jboolean writeToEndOfFile = DokanFileInfo->WriteToEndOfFile;
    jint allocationUnitSize = (jint) DokanFileInfo->DokanOptions->AllocationUnitSize;
    jlong globalContext = (jlong) DokanFileInfo->DokanOptions->GlobalContext;
    jstring mountPoint = (*env)->NewString(env, DokanFileInfo->DokanOptions->MountPoint, (jsize) wcslen(DokanFileInfo->DokanOptions->MountPoint));
    jint options = (jint) DokanFileInfo->DokanOptions->Options;
    jint sectorSize = (jint) DokanFileInfo->DokanOptions->SectorSize;
    jboolean singleThread = DokanFileInfo->DokanOptions->SingleThread;
    jint timeout = (jint) DokanFileInfo->DokanOptions->Timeout;
    jstring uncName = (*env)->NewString(env, DokanFileInfo->DokanOptions->UNCName, (jsize) wcslen(DokanFileInfo->DokanOptions->UNCName));
    jshort version = (jshort) DokanFileInfo->DokanOptions->Version;
    jclass class = (*env)->FindClass(env, "one/papachi/dokany4j/results/DokanFileInfo");
    jmethodID constructor = (*env)->GetMethodID(env, class, "<init>", "(JZJZZZIZZIJLjava/lang/String;IIZILjava/lang/String;S)V");
    jobject result = (*env)->NewObject(env, class, constructor, context, deleteOnClose, dokanContext, isDirectory,
                                       nocache, pagingIo, processId, synchronousIo, writeToEndOfFile,
                                       allocationUnitSize, globalContext, mountPoint, options, sectorSize, singleThread,
                                       timeout, uncName, version);
    (*env)->DeleteLocalRef(env, mountPoint);
    (*env)->DeleteLocalRef(env, uncName);
    return result;
}

static void copy(JNIEnv *env, jstring srcString, LPWSTR dst, DWORD dstSize) {
    const jchar* src = (*env)->GetStringChars(env, srcString, NULL);
    jsize srcSize = (*env)->GetStringLength(env, srcString);
    jsize count = (jsize) (srcSize < dstSize ? srcSize : dstSize - 1);
    wmemcpy(dst, src, count);
    dst[count] = 0;
    (*env)->ReleaseStringChars(env, srcString, src);
}

static NTSTATUS DOKAN_CALLBACK Dokany4jCreateFile(LPCWSTR FileName, PDOKAN_IO_SECURITY_CONTEXT SecurityContext, ACCESS_MASK DesiredAccess, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jint shareAccess = (jint) ShareAccess;
    jint genericDesiredAccess;
    jint fileAttributesAndFlags;
    jint creationDisposition;
    DokanMapKernelToUserCreateFileFlags(DesiredAccess, FileAttributes, CreateOptions, CreateDisposition,
                                        (ACCESS_MASK*) &genericDesiredAccess, (DWORD*)(DWORD*) &fileAttributesAndFlags, (DWORD*) &creationDisposition);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "createFile", "(Ljava/lang/String;IIIILone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/CreateFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, genericDesiredAccess, fileAttributesAndFlags, shareAccess, creationDisposition, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "isDirectory", "()Z");
    jboolean isDirectory = (*env)->CallIntMethod(env, result, methodId);
    if (isDirectory == JNI_TRUE) {
        DokanFileInfo->IsDirectory = 1;
    }
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static void DOKAN_CALLBACK Dokany4jCloseFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "closeFile", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)V");
    (*env)->CallVoidMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
}

static void DOKAN_CALLBACK Dokany4jCleanup(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "cleanup", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)V");
    (*env)->CallVoidMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
}

static NTSTATUS DOKAN_CALLBACK Dokany4jReadFile(LPCWSTR FileName, LPVOID Buffer, DWORD BufferLength, LPDWORD ReadLength, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jobject buffer = (*env)->NewDirectByteBuffer(env, Buffer, BufferLength);
    jlong offset = Offset;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "readFile", "(Ljava/lang/String;Ljava/nio/ByteBuffer;JLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/ReadFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, buffer, offset, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "readLength", "()I");
    jint readLength = (*env)->CallIntMethod(env, result, methodId);
    *ReadLength = readLength;
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*env)->DeleteLocalRef(env, buffer);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jWriteFile(LPCWSTR FileName, LPCVOID Buffer, DWORD NumberOfBytesToWrite, LPDWORD NumberOfBytesWritten, LONGLONG Offset, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jobject buffer = (*env)->NewDirectByteBuffer(env, (LPVOID) Buffer, NumberOfBytesToWrite);
    jlong offset = Offset;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "writeFile", "(Ljava/lang/String;Ljava/nio/ByteBuffer;JLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/WriteFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, buffer, offset, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "numberOfBytesWritten", "()I");
    jint numberOfBytesWritten = (*env)->CallIntMethod(env, result, methodId);
    *NumberOfBytesWritten = numberOfBytesWritten;
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*env)->DeleteLocalRef(env, buffer);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jFlushFileBuffers(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "flushFileBuffers", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/FlushFileBuffers;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jGetFileInformation(LPCWSTR FileName, LPBY_HANDLE_FILE_INFORMATION HandleFileInformation, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "getFileInformation", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/GetFileInformationResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "dwFileAttributes", "()I");
    jint dwFileAttributes = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "dwVolumeSerialNumber", "()I");
    jint dwVolumeSerialNumber = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "ftCreationTime", "()J");
    jlong ftCreationTime = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "ftLastAccessTime", "()J");
    jlong ftLastAccessTime = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "ftLastWriteTime", "()J");
    jlong ftLastWriteTime = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "fileIndex", "()J");
    jlong fileIndex = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "fileSize", "()J");
    jlong fileSize = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "numberOfLinks", "()I");
    jint numberOfLinks = (*env)->CallIntMethod(env, result, methodId);
    HandleFileInformation->dwFileAttributes = dwFileAttributes;
    HandleFileInformation->dwVolumeSerialNumber = dwVolumeSerialNumber;
    jlongToFiletime(ftCreationTime, &HandleFileInformation->ftCreationTime);
    jlongToFiletime(ftLastAccessTime, &HandleFileInformation->ftLastAccessTime);
    jlongToFiletime(ftLastWriteTime, &HandleFileInformation->ftLastWriteTime);
    HandleFileInformation->nFileIndexHigh = fileIndex >> 32;
    HandleFileInformation->nFileIndexLow = fileIndex & 0xFFFFFFFF;
    HandleFileInformation->nFileSizeHigh = fileSize >> 32;
    HandleFileInformation->nFileSizeLow = fileSize & 0xFFFFFFFF;
    HandleFileInformation->nNumberOfLinks = numberOfLinks;
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jFindFiles(LPCWSTR FileName, PFillFindData FillFindData, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "findFiles", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/FindFilesResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "result", "()Ljava/util/Iterator;");
    jobject iterator = (*env)->CallObjectMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, iterator);
    jmethodID methodIdHasNext = (*env)->GetMethodID(env, class, "hasNext", "()Z");
    jmethodID methodIdNext = (*env)->GetMethodID(env, class, "next", "()Ljava/lang/Object;");
    WIN32_FIND_DATAW FindData;
    while ((*env)->CallBooleanMethod(env, iterator, methodIdHasNext) == JNI_TRUE) {
        jobject findData = (*env)->CallObjectMethod(env, iterator, methodIdNext);
        class = (*env)->GetObjectClass(env, findData);
        methodId = (*env)->GetMethodID(env, class, "fileName", "()Ljava/lang/String;");
        jstring filename = (*env)->CallObjectMethod(env, findData, methodId);
        methodId = (*env)->GetMethodID(env, class, "dwFileAttributes", "()I");
        jint dwFileAttributes = (*env)->CallIntMethod(env, findData, methodId);
        methodId = (*env)->GetMethodID(env, class, "ftCreationTime", "()J");
        jlong ftCreationTime = (*env)->CallLongMethod(env, findData, methodId);
        methodId = (*env)->GetMethodID(env, class, "ftLastAccessTime", "()J");
        jlong ftLastAccessTime = (*env)->CallLongMethod(env, findData, methodId);
        methodId = (*env)->GetMethodID(env, class, "ftLastWriteTime", "()J");
        jlong ftLastWriteTime = (*env)->CallLongMethod(env, findData, methodId);
        methodId = (*env)->GetMethodID(env, class, "fileSize", "()J");
        jlong fileSize = (*env)->CallLongMethod(env, findData, methodId);
        copy(env, filename, (LPWSTR) &FindData.cFileName, 260);
        FindData.cAlternateFileName[0] = 0;
        FindData.dwFileAttributes = dwFileAttributes;
        jlongToFiletime(ftCreationTime, &FindData.ftCreationTime);
        jlongToFiletime(ftLastAccessTime, &FindData.ftLastAccessTime);
        jlongToFiletime(ftLastWriteTime, &FindData.ftLastWriteTime);
        FindData.nFileSizeHigh = fileSize >> 32;
        FindData.nFileSizeLow = fileSize & 0xFFFFFFFF;
        FindData.dwReserved0 = 0;
        FindData.dwReserved1 = 0;
        FillFindData(&FindData, DokanFileInfo);
        (*env)->DeleteLocalRef(env, filename);
        (*env)->DeleteLocalRef(env, findData);
    }
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jDeleteFile(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "deleteFile", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/DeleteFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jDeleteDirectory(LPCWSTR FileName, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "deleteDirectory", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/DeleteDirectoryResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jMoveFile(LPCWSTR FileName, LPCWSTR NewFileName, BOOL ReplaceIfExisting, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jstring newFileName = (*env)->NewString(env, NewFileName, (jsize) wcslen(NewFileName));
    jboolean replaceIfExisting = ReplaceIfExisting == FALSE ? JNI_FALSE : JNI_TRUE;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "moveFile", "(Ljava/lang/String;Ljava/lang/String;ZLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/MoveFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, newFileName, replaceIfExisting, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*env)->DeleteLocalRef(env, newFileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jLockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jlong offset = ByteOffset;
    jlong length = Length;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "lockFile", "(Ljava/lang/String;JJLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/LockFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, offset, length, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jSetEndOfFile(LPCWSTR FileName, LONGLONG ByteOffset, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jlong offset = ByteOffset;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "setEndOfFile", "(Ljava/lang/String;JLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/SetEndOfFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, offset, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jSetAllocationSize(LPCWSTR FileName, LONGLONG AllocSize, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jlong allocSize = AllocSize;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "setAllocationSize", "(Ljava/lang/String;JLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/SetAllocationSizeResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, allocSize, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jSetFileAttributes(LPCWSTR FileName, DWORD FileAttributes, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jint fileAttributes = (jint) FileAttributes;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "setFileAttributes", "(Ljava/lang/String;ILone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/SetFileAttributesResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, fileAttributes, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jSetFileTime(LPCWSTR FileName, CONST FILETIME *CreationTime, CONST FILETIME *LastAccessTime, CONST FILETIME *LastWriteTime, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jlong creationTime = filetimeToJLong(*CreationTime);
    jlong lastAccessTime = filetimeToJLong(*LastAccessTime);
    jlong lastWriteTime = filetimeToJLong(*LastWriteTime);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "setFileTime", "(Ljava/lang/String;JJJLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/SetFileTimeResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, creationTime, lastAccessTime, lastWriteTime, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jUnlockFile(LPCWSTR FileName, LONGLONG ByteOffset, LONGLONG Length, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jlong offset = ByteOffset;
    jlong length = Length;
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "unlockFile", "(Ljava/lang/String;JJLone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/UnlockFileResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, offset, length, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jGetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG BufferLength, PULONG LengthNeeded, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jint securityInformation = (jint) *SecurityInformation;
    jobject security = (*env)->NewDirectByteBuffer(env, SecurityDescriptor, BufferLength);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "getFileSecurity", "(Ljava/lang/String;ILjava/nio/ByteBuffer;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/GetFileSecurityResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, securityInformation, security, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "lengthNeeded", "()I");
    jint lengthNeeded = (*env)->CallIntMethod(env, result, methodId);
    *LengthNeeded = lengthNeeded;
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*env)->DeleteLocalRef(env, security);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jSetFileSecurity(LPCWSTR FileName, PSECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG SecurityDescriptorLength, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jstring fileName = (*env)->NewString(env, FileName, (jsize) wcslen(FileName));
    jint securityInformation = (jint) *SecurityInformation;
    jobject security = (*env)->NewDirectByteBuffer(env, SecurityDescriptor, SecurityDescriptorLength);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "setFileSecurity", "(Ljava/lang/String;ILjava/nio/ByteBuffer;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/SetFileSecurityResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, fileName, securityInformation, security, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, fileName);
    (*env)->DeleteLocalRef(env, security);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jGetVolumeInformation(LPWSTR VolumeNameBuffer, DWORD VolumeNameSize, LPDWORD VolumeSerialNumber, LPDWORD MaximumComponentLength, LPDWORD FileSystemFlags, LPWSTR FileSystemNameBuffer, DWORD FileSystemNameSize, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "getVolumeInformation", "(Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/GetVolumeInformationResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "volumeName", "()Ljava/lang/String;");
    jstring volumeName = (*env)->CallObjectMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "volumeSerialNumber", "()I");
    jint volumeSerialNumber = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "maximumComponentLength", "()I");
    jint maximumComponentLength = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "fileSystemFlags", "()I");
    jint fileSystemFlags = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "fileSystemName", "()Ljava/lang/String;");
    jstring fileSystemName = (*env)->CallObjectMethod(env, result, methodId);
    copy(env, volumeName, VolumeNameBuffer, VolumeNameSize);
    *VolumeSerialNumber = volumeSerialNumber;
    *MaximumComponentLength = maximumComponentLength;
    *FileSystemFlags = fileSystemFlags;
    copy(env, fileSystemName, FileSystemNameBuffer, FileSystemNameSize);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, volumeName);
    (*env)->DeleteLocalRef(env, fileSystemName);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jDokanGetDiskFreeSpace(PULONGLONG FreeBytesAvailable, PULONGLONG TotalNumberOfBytes, PULONGLONG TotalNumberOfFreeBytes, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "getDiskFreeSpace", "(Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/GetDiskFreeSpaceResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "freeBytesAvailable", "()J");
    jlong freeBytesAvailable = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "totalNumberOfBytes", "()J");
    jlong totalNumberOfBytes = (*env)->CallLongMethod(env, result, methodId);
    methodId = (*env)->GetMethodID(env, class, "totalNumberOfFreeBytes", "()J");
    jlong totalNumberOfFreeBytes = (*env)->CallLongMethod(env, result, methodId);
    *FreeBytesAvailable = freeBytesAvailable;
    *TotalNumberOfBytes = totalNumberOfBytes;
    *TotalNumberOfFreeBytes = totalNumberOfFreeBytes;
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jFindStreams(LPCWSTR FileName, PFillFindStreamData FillFindStreamData, PVOID FindStreamContext, PDOKAN_FILE_INFO DokanFileInfo) {
    return STATUS_NOT_IMPLEMENTED;
}

static NTSTATUS DOKAN_CALLBACK Dokany4jMounted(LPCWSTR MountPoint, PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jstring mountPoint = (*env)->NewString(env, MountPoint, (jsize) wcslen(MountPoint));
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "mounted", "(Ljava/lang/String;Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/MountedResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, mountPoint, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*env)->DeleteLocalRef(env, mountPoint);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
 }

static NTSTATUS DOKAN_CALLBACK Dokany4jUnmounted(PDOKAN_FILE_INFO DokanFileInfo) {
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(jvm, (void**) &env, NULL);
    jobject callbackObject = (jobject) DokanFileInfo->DokanOptions->GlobalContext;
    jobject dokanFileInfo = javaDokanFileInfo(env, DokanFileInfo);
    jclass class = (*env)->GetObjectClass(env, callbackObject);
    jmethodID methodId = (*env)->GetMethodID(env, class, "unmounted", "(Lone/papachi/dokany4j/results/DokanFileInfo;)Lone/papachi/dokany4j/results/UnmountedResult;");
    jobject result = (*env)->CallObjectMethod(env, callbackObject, methodId, dokanFileInfo);
    (*env)->DeleteLocalRef(env, class);
    class = (*env)->GetObjectClass(env, result);
    methodId = (*env)->GetMethodID(env, class, "status", "()I");
    jint status = (*env)->CallIntMethod(env, result, methodId);
    (*env)->DeleteGlobalRef(env, callbackObject);
    (*env)->DeleteLocalRef(env, class);
    (*env)->DeleteLocalRef(env, result);
    (*env)->DeleteLocalRef(env, dokanFileInfo);
    (*jvm)->DetachCurrentThread(jvm);
    return status;
}

JNIEXPORT jint JNICALL Java_one_papachi_dokany4j_Dokany4j_mount(JNIEnv *env, jobject thisObj, jint version,
                                                                  jboolean singleThread, jint options, jstring mountPoint,
                                                                  jstring uncName, jint timeout,
                                                                  jint allocationUnitSize, jint sectorSize) {
    (*env)->GetJavaVM(env, &jvm);

    DOKAN_OPTIONS DokanOptions;
    DokanOptions.Version = version;
    DokanOptions.SingleThread = singleThread;
    DokanOptions.Options = options;
    jint mountPointLength = (*env)->GetStringLength(env, mountPoint);
    LPWSTR MountPoint = malloc((mountPointLength + 1) * sizeof(WCHAR));
    copy(env, mountPoint, MountPoint, mountPointLength + 1);
    DokanOptions.MountPoint = MountPoint;
    jint uncNameLength = (*env)->GetStringLength(env, uncName);
    LPWSTR UNCName = malloc((uncNameLength + 1) * sizeof(WCHAR));
    copy(env, uncName, UNCName, uncNameLength + 1);
    DokanOptions.UNCName = UNCName;
    DokanOptions.Timeout = timeout;
    DokanOptions.AllocationUnitSize = allocationUnitSize;
    DokanOptions.SectorSize = sectorSize;
    DokanOptions.VolumeSecurityDescriptorLength = 0;
    jobject callbackObject = (*env)->NewGlobalRef(env, thisObj);
    DokanOptions.GlobalContext = (ULONG64) callbackObject;

    DOKAN_OPERATIONS DokanOperations;
    DokanOperations.ZwCreateFile = Dokany4jCreateFile;
    DokanOperations.Cleanup = Dokany4jCleanup;
    DokanOperations.CloseFile = Dokany4jCloseFile;
    DokanOperations.ReadFile = Dokany4jReadFile;
    DokanOperations.WriteFile = Dokany4jWriteFile;
    DokanOperations.FlushFileBuffers = Dokany4jFlushFileBuffers;
    DokanOperations.GetFileInformation = Dokany4jGetFileInformation;
    DokanOperations.FindFiles = Dokany4jFindFiles;
    DokanOperations.FindFilesWithPattern = NULL;
    DokanOperations.SetFileAttributes = Dokany4jSetFileAttributes;
    DokanOperations.SetFileTime = Dokany4jSetFileTime;
    DokanOperations.DeleteFile = Dokany4jDeleteFile;
    DokanOperations.DeleteDirectory = Dokany4jDeleteDirectory;
    DokanOperations.MoveFile = Dokany4jMoveFile;
    DokanOperations.SetEndOfFile = Dokany4jSetEndOfFile;
    DokanOperations.SetAllocationSize = Dokany4jSetAllocationSize;
    DokanOperations.LockFile = Dokany4jLockFile;
    DokanOperations.UnlockFile = Dokany4jUnlockFile;
    DokanOperations.GetFileSecurity = Dokany4jGetFileSecurity;
    DokanOperations.SetFileSecurity = Dokany4jSetFileSecurity;
    DokanOperations.GetDiskFreeSpace = Dokany4jDokanGetDiskFreeSpace;
    DokanOperations.GetVolumeInformation = Dokany4jGetVolumeInformation;
    DokanOperations.Unmounted = Dokany4jUnmounted;
    DokanOperations.FindStreams = Dokany4jFindStreams;
    DokanOperations.Mounted = Dokany4jMounted;

    DokanInit();
    return DokanMain(&DokanOptions, &DokanOperations);
}

JNIEXPORT void JNICALL Java_one_papachi_dokany4j_Dokany4j_unmount(JNIEnv *env, jclass thisObj, jstring mountPoint) {
    jint mountPointLength = (*env)->GetStringLength(env, mountPoint);
    LPWSTR MountPoint = malloc((mountPointLength + 1) * sizeof(WCHAR));
    copy(env, mountPoint, MountPoint, mountPointLength + 1);
    DokanRemoveMountPoint(MountPoint);
}
