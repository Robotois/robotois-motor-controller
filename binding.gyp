{
  "targets": [
    {
      "target_name": "MotorsModule",
      "sources": [
        "src/wrapper/MotorsModule.cpp",
        "src/wrapper/MotorsWrapper.cpp",
        "src/Motors/Motors.cpp"
      ],
      "libraries": ["-l bcm2835"]
    }
  ]
}
