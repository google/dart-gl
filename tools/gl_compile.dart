// Public Domain
// https://github.com/mezoni/ccompile

library ccompile.example.example_build;

import 'dart:convert';
import 'dart:io';

import 'package:ccompile/ccompile.dart';
import 'package:logging/logging.dart';
import 'package:path/path.dart' as pathos;

void main(List<String> args) {
  Program.main(args);
}

class Program {
  static int buildProject(projectPath, Map messages) {
    var workingDirectory = pathos.dirname(projectPath);
    var message = messages['start'];
    if (!message.isEmpty) {
      print(message);
    }

    var logger = new Logger("Builder");
    logger.onRecord.listen((record) {
      try {
        var decoder = new JsonDecoder();
        var message = decoder.convert(record.message);
        if (message is Map) {
          if (message["operation"] == "run") {
            var parameters = message["parameters"];
            if (parameters is Map) {
              var executable = parameters["executable"];
              if (executable is String) {
                var arguments = parameters["arguments"];
                if (arguments is List) {
                  print("$executable ${arguments.join(" ")}");
                }
              }
            }
          }
        }
      } catch (e) {}
    });

    ProjectBuilder.logger = logger;
    var builder = new ProjectBuilder();
    var project = builder.loadProject(projectPath);
    var result = builder.buildAndClean(project, workingDirectory);
    if (result.exitCode == 0) {
      var message = messages['success'];
      if (!message.isEmpty) {
        print(message);
      }
    } else {
      var message = messages['error'];
      if (!message.isEmpty) {
        print(message);
      }
    }

    return result.exitCode == 0 ? 0 : 1;
  }

  static String getRootScriptDirectory() {
    return pathos.dirname(Platform.script.toFilePath());
  }

  static void main(List<String> args) {
    if (args.isEmpty) args = ['gl_extension.yaml'];
    var basePath = Directory.current.path;
    var projectPath = toAbsolutePath(args[0], basePath);
    var result = Program.buildProject(projectPath, {
      'start': 'Building project "$projectPath"',
      'success': 'Building complete successfully',
      'error': 'Building complete with some errors'
    });

    exit(result);
  }

  static String toAbsolutePath(String path, String base) {
    if (pathos.isAbsolute(path)) {
      return path;
    }

    path = pathos.join(base, path);
    return pathos.absolute(path);
  }
}
