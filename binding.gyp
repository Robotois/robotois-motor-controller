{
  "targets": [
    {
      "target_name": "MotorsModule",
      "sources": [ "MotorsModule.cpp","MotorsWrapper.cpp",
      "src/Modules/Motors/Motors.cpp",
      "src/Libraries/Timer/AccurateTiming.cpp"
      ],
      "libraries": ["-l bcm2835","-l rt"]
    }
  ]
}
