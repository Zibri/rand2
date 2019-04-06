./rand2 |ffmpeg -f u16be -ar 48000 -ac 1 -i - -f rawvideo -pixel_format yuv420p -video_size 1920x1080 -framerate 60 -i - -c:a ac3 -vcodec copy -f avi - |ffplay -fs -volume 10 -
