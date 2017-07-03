{
  "targets": [
    {
      "target_name": "MotorsModule",
      "sources": [
        "src/wrapper/MotorsModule.cpp",
        "src/wrapper/MotorsWrapper.cpp",
        "src/Motors.cpp"
      ],
      "libraries": ["-l bcm2835","-l rt"]
    }
  ]
}
