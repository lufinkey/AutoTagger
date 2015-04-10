
#include "Tagger.h"
#include <taglib/mpeg/mpegfile.h>
#include <taglib/mpeg/id3v2/id3v2tag.h>
#include <taglib/mpeg/id3v2/frames/textidentificationframe.h>
#include <taglib/mpeg/id3v2/frames/attachedpictureframe.h>

namespace autotagger
{
	void tag_albumartist(TagLib::MPEG::File& file, const ScrapedData&scrapedData, const TagOptions&options)
	{
		std::string albumartist = scrapedData.album_properties.artist;
		TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
		if(!tag->frameList("TPE2").isEmpty())
		{
			tag->frameList("TPE2").front()->setText(albumartist);
		}
		else
		{
			TagLib::ID3v2::TextIdentificationFrame *frame = new TagLib::ID3v2::TextIdentificationFrame("TPE2", TagLib::String::UTF8);
			tag->addFrame(frame);
			frame->setText(albumartist);
		}
	}
	
	void tag_albumartwork(TagLib::MPEG::File& file, const ScrapedData&scrapedData, const TagOptions&options)
	{
		if(scrapedData.album_properties.artwork.size()!=0 && scrapedData.album_properties.artwork_mimetype.length()!=0)
		{
			TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
			TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
			TagLib::ID3v2::AttachedPictureFrame *frame = nullptr;
			if(frames.isEmpty())
			{
				frame = new TagLib::ID3v2::AttachedPictureFrame();
				tag->addFrame(frame);
			}
			else
			{
				frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
			}
			
			frame->setPicture(TagLib::ByteVector(scrapedData.album_properties.artwork.data(), scrapedData.album_properties.artwork.size()));
			frame->setMimeType(scrapedData.album_properties.artwork_mimetype);
			frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
		}
	}
	
	bool Tagger::tagFile(const ScrapedData&scrapedData, unsigned int trackIndex, const std::string& filePath, const TagOptions&options)
	{
		const TrackProperties* track_ptr = nullptr;
		size_t track_properties_size = scrapedData.track_properties.size();
		for(size_t i=0; i<track_properties_size; i++)
		{
			track_ptr = &scrapedData.track_properties[i];
			try
			{
				if(track_ptr->index == trackIndex)
				{
					i = track_properties_size;
				}
			}
			catch(const std::out_of_range&)
			{
				track_ptr = nullptr;
			}
		}
		
		if(track_ptr != nullptr)
		{
			const TrackProperties& track = *track_ptr;
			TagLib::MPEG::File file(filePath.c_str());
			TagLib::ID3v2::Tag*tag = file.ID3v2Tag(true);
			file.tag()->setAlbum(scrapedData.album_properties.title);
			tag_albumartist(file, scrapedData, options);
			tag_albumartwork(file, scrapedData, options);
			file.tag()->setGenre(scrapedData.album_properties.genre);
			file.tag()->setYear(scrapedData.album_properties.year);
			file.tag()->setTrack(track.index);
			file.tag()->setTitle(track.title);
			file.tag()->setArtist(track.artist);
			tag->frameList("TRCK").front()->setText(std::to_string(track.index) + "/" + std::to_string(scrapedData.album_properties.tracks));
			
			if(options.clearUnusedFields)
			{
				file.tag()->setComment("");
				//TODO clear lyrics
			}
			
			file.save();
			return true;
		}
		return false;
	}
	
	size_t Tagger::tagFiles(const ScrapedData&scrapedData, const std::vector<std::pair<unsigned int, std::string> >& files, const TagOptions&options)
	{
		size_t taggedFiles = 0;
		size_t files_size = files.size();
		for(size_t i=0; i<files_size; i++)
		{
			bool success = tagFile(scrapedData, files.at(i).first, files.at(i).second, options);
			if(success)
			{
				taggedFiles++;
			}
		}
		return taggedFiles;
	}
}
