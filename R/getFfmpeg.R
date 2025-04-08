getFfmpeg <-  function(){
  statusFfmpeg <-  system("ffmpeg -v", show.output.on.console = F)
  if (statusFfmpeg == 127){
    download.file("https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip", destfile = "ffmpeg.zip")
    allfiles <- list.files()
    unzip("ffmpeg.zip")
    newfile <- setdiff(list.files(), allfiles)
  
    file.copy(file.path(newfile,"/bin/ffmpeg.exe"), "ffmpeg.exe")
    file.remove("ffmpeg.zip")
    file.remove("./ffmpeg-7.1.1-essentials_build", recursive = T)
  } else {
    print("ffmpeg available, no need to download and install")
  }
}
