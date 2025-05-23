getFfmpeg <-  function(){
  suppressWarnings({
    statusFfmpeg <-  system("ffmpeg -v", show.output.on.console = F, intern = FALSE, ignore.stdout = TRUE, ignore.stderr = TRUE)
    if (statusFfmpeg == 127){

      oldTimeOut <- getOption("timeout")
      options(timeout = 300)
      download.file("https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip", destfile = "ffmpeg.zip")
      allfiles <- list.files()
      unzip("ffmpeg.zip")
      newfile <- setdiff(list.files(), allfiles)
  
      file.copy(file.path(newfile,"/bin/ffmpeg.exe"), "ffmpeg.exe")
      file.remove("ffmpeg.zip")
      file.remove("./ffmpeg-7.1.1-essentials_build", recursive = T, showWarnings = FALSE)

      options(timeout = oldTimeOut)
    
    } else {
      print("ffmpeg available, no need to download and install")
    }
  })
} 
