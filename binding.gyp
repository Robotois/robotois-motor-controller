{
  "targets": [
    {
      "target_name": "MotorsModule",
      "sources": [ "src/Wrapper/MotorsModule.cpp","src/Wrapper/MotorsWrapper.cpp",
      "src/Modules/Motors/Motors.cpp",
      "src/Libraries/Timer/AccurateTiming.cpp"
      ],
      "libraries": ["-l bcm2835","-l rt"]
    }
  ]
}
