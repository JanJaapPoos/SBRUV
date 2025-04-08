extractFramesDir <- function(img.dir='calibration_videos', output.dir = NULL, pattern = "mov" ){
  if (is.null(output.dir)) output.dir1 <- paste0(img.dir,"/Images")
  
  system(paste0('mkdir ',output.dir1))

  for(ii in list.files(img.dir, pattern = pattern, include.dirs = F)){
    print(ii)
    
    mov.name  <- strsplit(ii, "\\.")[[1]][1]
    output.dir2 <- paste0(output.dir1,"/",mov.name)
    system(paste0('mkdir ',output.dir2))
    
    cmd1 <- paste0("ffmpeg -i ",  img.dir,"\\", ii, " -vsync vfr ", output.dir2,"/%04d.jpg")
    system(cmd1, show.output.on.console = F)
  }
}
