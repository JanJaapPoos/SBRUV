extractFramesDir <- function (img.dir = "obs_videos", output.dir = NULL, 
          pattern = "MP4") 
{
  if (is.null(output.dir)) 
    output.dir <- "Images"
  dir.create(getwd(), img.dir, output.dir)
  for (ii in list.files(img.dir, pattern = pattern, include.dirs = F)) {
    print(ii)
    mov.name <- strsplit(ii, "\\.")[[1]][1]
    output.dir2 <- paste0(getwd(),"/",img.dir,"/",output.dir, "/", mov.name)
    dir.create(getwd(), img.dir, output.dir, mov.name)
    cmd1 <- paste0("ffmpeg -i ", img.dir, "\\", ii, " -vsync vfr ", 
                   output.dir2, "/%04d.jpg")
    system(cmd1, show.output.on.console = F)
  }
}
