extractFramesDir <- function (img.dir = "obs_videos", output.dir = NULL, pattern = "MP4", nth = 1) {
  if (is.null(output.dir)) 
    output.dir <- "Images"
  
  dir.create(file.path(getwd(), img.dir, output.dir))

  for (ii in list.files(img.dir, pattern = pattern, ignore.case= TRUE, include.dirs = FALSE)) {
    print(ii)
    mov.name <- strsplit(ii, "\\.")[[1]][1]
    output.dir2 <- file.path(img.dir, output.dir, mov.name)
    dir.create(file.path(getwd(), img.dir, output.dir, mov.name))
   
    cmd1 <- paste0(
      'ffmpeg -i "', img.dir, '/', ii, 
      '" -vf "select=not(mod(n\\,', nth, '))"',
      ' -fps_mode passthrough',
      ' -frame_pts 1 "',
      output.dir2, '/%05d.jpg"'
    )
    
    system(cmd1, show.output.on.console = F)
  }
}
