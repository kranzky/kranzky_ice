#!/usr/bin/env ruby

require "fileutils"
require "zip/zip"
require "zip/zipfilesystem"

def buildResources( target )
    resources_file = File.join( target, "resources.dat" )
    FileUtils.rm_f( resources_file )
    Zip::ZipFile.open( resources_file, Zip::ZipFile::CREATE) do |zipfile|
        Dir.entries( "Resources" ).each do |filename|
            next unless filename =~ /\.(png|mod|res|fnt|psi|wav|mp3)$/
            zipfile.add( filename, File.join( "Resources", filename ) )
        end
    end
    FileUtils.cp( File.join( "Resources", "scores.db3" ), target );
    third_party = File.join( "..", "..", "ThirdParty" )
    FileUtils.cp( File.join( third_party, "hge", "hge.dll" ), target )
    FileUtils.cp( File.join( third_party, "hge", "bass.dll" ), target )
    FileUtils.cp( File.join( third_party, "sqlite", "sqlite3.dll" ), target )
    FileUtils.cp( File.join( third_party, "libcurl", "libcurl.dll" ), target )
    FileUtils.cp( File.join( third_party, "libcurl", "zlib1.dll" ), target )
    FileUtils.cp( File.join( third_party, "XenonPad", "xinput1_3.dll" ), target )
end

if __FILE__ == $0
    target = "."
    target = ARGV[0] if ARGV.length > 0
    buildResources( target )
end
