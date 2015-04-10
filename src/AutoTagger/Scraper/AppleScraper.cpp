
#include "AppleScraper.h"

namespace autotagger
{
	ScrapedData AppleScraper::scrape(const std::string&pagedata)
	{
		xmlpp::Document*doc = new xmlpp::Document(htmlReadDoc((xmlChar*)pagedata.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING));
		
		xmlpp::Element* content = xmlpp::Node_getElementById(doc->get_root_node(), "content");
		xmlpp::Element* title_box = xmlpp::Node_getElementById(content, "title");
		
		ScrapedData scrapedData;
		xmlpp::Element* title_box_left = xmlpp::Node_getElementsByClassName(title_box, "left").at(0);
		scrapedData.album_properties.title = xmlpp::Node_getFirstChildElement(title_box_left)->get_child_text()->get_content();
		scrapedData.album_properties.artist = xmlpp::Node_getNthChildElement(title_box_left, 2)->get_child_text()->get_content();
		
		xmlpp::Element* tracklist_content_box = xmlpp::Node_getElementsByClassName(content,"tracklist-content-box").at(0);
		xmlpp::Element* tracklist_table = xmlpp::Node_getElementsByClassName(tracklist_content_box, "tracklist-table").at(0);
		
		xmlpp::Element* table_body = xmlpp::Node_getNthChildElement(tracklist_table, 2);
		xmlpp::Node::NodeList rows = table_body->get_children("tr");
		
		unsigned int highest_track_index = 0;
		
		for(size_t i=0; i<rows.size(); i++)
		{
			xmlpp::Element* row = static_cast<xmlpp::Element*>(*std::next(rows.begin(), i));
			xmlpp::Node::NodeList row_data = row->get_children("td");
			
			std::vector<std::string> row_data_strs;
			
			for(size_t j=0; j<row_data.size(); j++)
			{
				std::string txt = xmlpp::Node_getElementsByTagName(xmlpp::Node_getFirstChildElement(*std::next(rows.begin(), i)), "span").at(0)->get_child_text()->get_content();
				row_data_strs.push_back(txt);
			}
			
			TrackProperties track;
			track.index = (unsigned int)std::atoi(row_data_strs[0].c_str());
			track.title = row_data_strs[1];
			track.artist = row_data_strs[2];
			scrapedData.track_properties.push_back(track);
			
			if(track.index > highest_track_index)
			{
				highest_track_index = track.index;
			}
		}
		
		scrapedData.album_properties.tracks = highest_track_index;
		
		delete doc;
		
		return scrapedData;
	}
}
