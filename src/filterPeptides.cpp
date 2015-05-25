/*
 * filterPeptides.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: geforce
 */

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <pugixml/pugixml.hpp>

std::unordered_set<std::string> loadBlacklist(std::string file);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << std::endl
				<< "Filter peptides is used to filter pep.xml files from blacklisted peptides after the search has been performed. For example shared peptides between two species or known containments."
				<< std::endl;
		std::cerr << "Usage: filterPeptides [file.pep.xml] [blacklist]" << std::endl;
		exit(1);
	}
	//Load the blacklist in our hashmap for fast acces
	std::unordered_set<std::string> blacklistSet(loadBlacklist(argv[2]));
	std::cout << "Blacklist (" << argv[2] << ") contains " << blacklistSet.size() << " peptides" << std::endl;

	std::string inFile(argv[1]);
	std::string extension(".pep.xml");
	std::string outFile(inFile.substr(0, inFile.length() - extension.length()));
	outFile.append(".filtered.pep.xml");
	std::ofstream outputStream(outFile, std::ofstream::out | std::ofstream::trunc);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(argv[1]);

	if (result)
	{
		pugi::xml_object_range<pugi::xml_named_node_iterator> spectrumQueries =
				doc.child("msms_pipeline_analysis").child("msms_run_summary").children("spectrum_query");
		std::cout << "pugixml: [" << argv[1] << "] parsed without errors, total spectrum queries: "
				<< std::distance(spectrumQueries.begin(), spectrumQueries.end()) << "\n";
	}
	else
	{
		std::cout << "pugixml: [" << argv[1] << "] parsed with errors\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset;
		exit(1);
	}

	unsigned totalDeleted = 0;
	unsigned totalPeptides = 0;
	//Containers to store our nodes we need to delete afterwards
	std::vector<pugi::xml_node> spectrumToDelete;
	std::vector<pugi::xml_node> searchhitToDelete;
	//Initialize the searchhitNodes counter
	unsigned searchhitNodes = 0;

	for (pugi::xml_node spectrum : doc.child("msms_pipeline_analysis").child("msms_run_summary").children(
			"spectrum_query"))
	{
		//Temp variable for easier acces
		pugi::xml_object_range<pugi::xml_named_node_iterator> search_resultNodes =
				spectrum.child("search_result").children("search_hit");

		for (pugi::xml_node search_hit : search_resultNodes)
		{
			if (blacklistSet.find(search_hit.attribute("peptide").as_string()) != blacklistSet.end())
			{
				//notify the user of our node deletion
				std::cout << "Deleting from spectrum '" << spectrum.attribute("spectrum").as_string();
				std::cout << "' peptide hit '" << search_hit.attribute("peptide").as_string();
				std::cout << "' (rank:" << search_hit.attribute("hit_rank").as_uint() << ")" << "\n";
				searchhitToDelete.push_back(search_hit);

			}
			totalPeptides++;
		}

		if (searchhitToDelete.size() == 0)
		{
			//Short circuit and skip expensive deletion of nodes
			continue;
		}

		//Count the total amount of nodes
		searchhitNodes = std::distance(search_resultNodes.begin(), search_resultNodes.end());
		totalDeleted+=searchhitToDelete.size();

		if ((searchhitNodes - searchhitToDelete.size()) == 0)
		{
			//if the results are empty, delete the whole spectrum with it's query entries
			spectrumToDelete.push_back(spectrum);
		}
		else
		{
			//Remove the required nodes
			for (pugi::xml_node &search_hit : searchhitToDelete)
			{
				spectrum.child("search_result").remove_child(search_hit);
			}
			//TODO re-score the hit ranks of the other nodes
		}
		searchhitToDelete.clear();
	}

	//Clean up empty spectrum nodes
	for (pugi::xml_node &spectrum : spectrumToDelete)
	{
		doc.child("msms_pipeline_analysis").child("msms_run_summary").remove_child(spectrum);
	}

	//Pritn final results to the user
	std::cout << "Total peptides deleted: " << totalDeleted << " of " << totalPeptides << " peptide hits and removed "
			<< spectrumToDelete.size() << " empty spectrum_query results." << std::endl;
	std::cout << "Saving result: " << (doc.save_file(outFile.c_str(), "") ? outFile.c_str() : "failed") << std::endl
			<< std::endl;
}

std::unordered_set<std::string> loadBlacklist(std::string file)
{
	std::unordered_set<std::string> blacklistSet;
	std::ifstream blacklistFile(file);
	std::string line;
	if (blacklistFile.is_open())
	{
		std::size_t peptides = 0;
		while (std::getline(blacklistFile, line))
		{
			++peptides;
		}

		blacklistFile.clear();                 // clear fail and eof bits
		blacklistFile.seekg(0, std::ios::beg);

		blacklistSet.reserve(peptides);
		while (std::getline(blacklistFile, line))
		{
			if (line.find(" ", 0) != std::string::npos)
			{
				std::cerr << "Blacklist file contains multiple peptides per line. (" << line << ")" << std::endl;
				blacklistFile.close();
				exit(0);
			}

			blacklistSet.insert(line);
		}
		blacklistFile.close();
	}
	return blacklistSet;
}
