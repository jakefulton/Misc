#! /usr/bin/python3

from mutagen.mp3 import MP3
from mutagen.easyid3 import EasyID3
import re
import os

def rename_mp3_files(root_dir, artist_place=1, album_place=2, track_no_place=3,
                     title_place=4):
    """
    Renames all .mp3 files under root_dir

    New file name will contain artist, album, track#, and title
    New name default: "artist-album-track#-title

    Place values must be in range of 1-4
    Specify new file name ordering with different place values
    """
    ordered = {}
    for (root, dirs, files) in os.walk(path):
        for file in files:
            if (re.search('\.[Mm][Pp]3$', file)):
                #Get complete path for file
                old_name = os.path.join(root, file)

                #Get track information
                track = EasyID3(old_name)
                track_artist = track['artist'][0]
                track_album = track['album'][0]
                track_no = track['tracknumber'][0].split('/')[0].zfill(2)
                track_title = track['title'][0]

                #Keep track of how to order track information
                ordered[artist_place] = track_artist
                ordered[album_place] = track_album
                ordered[track_no_place] = track_no
                ordered[title_place] = track_title

                #Create new name
                new_name = os.path.join(root, "{}-{}-{}-{}.mp3".format(
                    ordered[1], ordered[2], ordered[3], ordered[4]))

                #Rename File
                os.rename(old_name, new_name)
                #print(new_name)


path = '/home/fultonj/Music'
if __name__ == "__main__":
    rename_mp3_files(path, 1, 2, 3, 4)
