/*
 * This code was auto generated by make_hipo2root.py
 * Do not modify this program.
 *
 *
 *
 * Author: Nick Tyler, University Of South Carolina
 */
// Standard libs
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>
// ROOT libs
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
// Hipo libs and clipp
#include "hipo4/reader.h"

int main(int argc, char** argv) {
  std::string InFileName;
  std::string OutFileName;
  if (argc >= 2) {
    InFileName = argv[1];
  } else {
    exit(1);
  }
  OutFileName = "pz_test.root";

  TFile* OutputFile      = new TFile(OutFileName.c_str(), "RECREATE");
  TH1D*  hist            = new TH1D("pz", "pz", 100, 0, 10);
  auto   reader          = std::make_shared<hipo::reader>(InFileName);
  size_t tot_hipo_events = reader->numEvents();

  auto dict = std::make_shared<hipo::dictionary>();
  reader->readDictionary(*dict);
  auto hipo_event   = std::make_shared<hipo::event>();
  auto rec_Particle = std::make_shared<hipo::bank>(dict->getSchema("REC::Particle"));

  int  entry      = 0;
  int  l          = 0;
  auto start_full = std::chrono::high_resolution_clock::now();
  while (reader->next() == true) {
    if (++entry % 10000 == 0)
      std::cout << " " << entry << "\r\r" << std::flush;

    reader->read(*hipo_event);
    hipo_event->getStructure(*rec_Particle);

    l = rec_Particle->getRows();
    if (l != -1) {
      for (int i = 0; i < l; i++) {
        if (rec_Particle->getFloat(3, i) < 10 && rec_Particle->getFloat(3, i) > -1 &&
            rec_Particle->getInt(0, i) == 11)
          hist->Fill(rec_Particle->getFloat(3, i));
      }
    }
  }
  OutputFile->cd();
  hist->Write();
  OutputFile->Close();

  return 0;
}
