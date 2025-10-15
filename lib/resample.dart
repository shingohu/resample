import 'dart:ffi';
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart' as ffi;

import 'resample_bindings_generated.dart';

const String _libName = 'resample';

/// The dynamic library in which the symbols for [ResampleBindings] can be found.
final DynamicLibrary _dylib = () {
  if (Platform.isMacOS || Platform.isIOS) {
    return DynamicLibrary.open('$_libName.framework/$_libName');
  }
  if (Platform.isAndroid || Platform.isLinux) {
    return DynamicLibrary.open('lib$_libName.so');
  }
  if (Platform.isWindows) {
    return DynamicLibrary.open('$_libName.dll');
  }
  if (Platform.operatingSystem == "ohos") {
    return DynamicLibrary.open('lib$_libName.so');
  }
  throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
}();

/// The bindings to the native functions in [_dylib].
final ResampleBindings _bindings = ResampleBindings(_dylib);

class Resample {
  Resample._();

  static Uint8List resample(Uint8List bytes, {required int inSampleRate, required int outSampleRate}) {
    return ffi.using((arena) {
      Int16List shorts = _bytesToShort(bytes);
      int length = shorts.length;
      final inputPtr = arena<Int16>(length);
      inputPtr.asTypedList(length).setAll(0, shorts);

      int outputLength = ((outSampleRate / inSampleRate) * length).toInt() + 1;

      final outPtr = arena<Int16>(outputLength);
      int ret = _bindings.resample_s16(inputPtr, outPtr, inSampleRate, outSampleRate, length, 1);
      if (ret > 0) {
        return _shortToBytes(outPtr.asTypedList(ret));
      } else {
        print(ret);
        print("处理失败");
        return bytes;
      }
    });
  }

  static Int16List _bytesToShort(Uint8List bytes) {
    Int16List shorts = Int16List(bytes.length ~/ 2);
    for (int i = 0; i < shorts.length; i++) {
      shorts[i] = (bytes[i * 2] & 0xff | ((bytes[i * 2 + 1] & 0xff) << 8));
    }
    return shorts;
  }

  static Uint8List _shortToBytes(Int16List shorts) {
    Uint8List bytes = Uint8List(shorts.length * 2);
    for (int i = 0; i < shorts.length; i++) {
      bytes[i * 2] = (shorts[i] & 0xff);
      bytes[i * 2 + 1] = (shorts[i] >> 8 & 0xff);
    }
    return bytes;
  }
}
