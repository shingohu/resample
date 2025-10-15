import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:resample/resample.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    super.initState();
  }

  Future<Uint8List> loadPCM({int? length}) async {
    ByteData byteData = await rootBundle.load("assets/16k.pcm");
    if (length != null && length > 0) {
      return byteData.buffer.asUint8List(0, length);
    }
    return byteData.buffer.asUint8List();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('PCM Resample'),
        ),
        body: SingleChildScrollView(
          child: Container(
              padding: const EdgeInsets.all(10),
              child: Column(
                children: [
                  TextButton(
                      onPressed: () async {
                        Uint8List bytes = await loadPCM();
                        Uint8List newBytes = Resample.resample(bytes, inSampleRate: 16000, outSampleRate: 8000);
                      },
                      child: Text('test')),
                ],
              )),
        ),
      ),
    );
  }
}
