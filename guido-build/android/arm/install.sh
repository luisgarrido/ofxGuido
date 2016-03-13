mkdir -p ../../../libs/guido/{lib/android/{armeabi,armeabi-v7a},include}
cp obj/local/armeabi/libGUIDOEngine.a ../../../libs/guido/lib/android/armeabi/
cp obj/local/armeabi-v7a/libGUIDOEngine.a ../../../libs/guido/lib/android/armeabi-v7a/
cp ../../../guidolib-code/src/engine/{include,devices,lib}/*.h ../../../libs/guido/include/

