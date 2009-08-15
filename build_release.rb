#!/usr/bin/env ruby

require "fileutils"
require "zip/zip"
require "zip/zipfilesystem"
require 'build_resources'

NAME = "project_ice"
PATH = "../../Releases"
number = 1

Dir.foreach(PATH) do |file|
    path = File.join(PATH, file)
    next unless File.file?(path)
    next unless file =~ /^#{NAME}\_([\d]+)\.zip$/
    old_number = Regexp.last_match(1).to_i
    number= old_number + 1 if old_number >= number
end

filename = File.join(PATH, sprintf("#{NAME}_%02d.zip", number))
FileUtils.rm_f( filename )

Zip::ZipFile.open( filename, Zip::ZipFile::CREATE) do |zipfile|
    zipfile.add( "readme.txt", "readme.txt" )
    zipfile.add( "resources.dat", File.join( "Release", "resources.dat" ) )
    zipfile.add( "scores.db3", File.join( "Release", "scores.db3" ) )
    zipfile.add( "ProjectIce.exe",
                 File.join( "Release", "ProjectIce.exe" ) )
    zipfile.add( "hge.dll", File.join( "Release", "hge.dll" ) )
    zipfile.add( "bass.dll", File.join( "Release", "bass.dll" ) )
    zipfile.add( "sqlite3.dll", File.join( "Release", "sqlite3.dll" ) )
    zipfile.add( "libcurl.dll", File.join( "Release", "libcurl.dll" ) )
    zipfile.add( "zlib1.dll", File.join( "Release", "zlib1.dll" ) )
    zipfile.add( "xinput1_3.dll", File.join( "Release", "xinput1_3.dll" ) )
end
