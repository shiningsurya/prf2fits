#ifndef _THIRD_H
#include "third.hpp"
#endif

using namespace std;
class PROFIO {
		private:
				FILE * file;
				int numchans,numbins;
				struct fromHeader * fh;
		public:
				bool MoreThere(){
						// return true if EOF is reached in file 
						return !feof(file);
				}
				PROFIO(){
						file = NULL;
						fh = (struct fromHeader*)malloc(sizeof(struct fromHeader));
				}
				PROFIO(string fn, int nc){
						file = fopen(fn.c_str(),"r");
						if(file == NULL) cerr << "PROFIO unable to open file " << fn << endl;
						fh = (struct fromHeader*)malloc(sizeof(struct fromHeader));
						numchans = nc;
						numbins = 0;
				}
				~PROFIO(){
						fclose(file);
						if(fh != NULL)  free(fh);
				}
				int readHeader(){
						if(!MoreThere()) return 1;
						if(fh == NULL) cerr << "Malloc failed while reading header\n";
						fscanf(file,"# %lf %lf %lf %ld %f %f %d %c %d \n",&(fh->mjd),&(fh->fract),&(fh->period),&(fh->numpulses),&(fh->freq),&(fh->dm),&(fh->numbins),&(fh->tid),&(fh->pol));
						numbins = fh->numbins;
						return 0;
				}
				// This function is anyway not called
				void isubs(int nc){
						numchans = nc;				// ret is not initialized
				}
				void readSubs(float * subway) {
						/*
						 * As of now, the idea is to allocate memory in the main class FITS. 
						 * Here, just reading is planned to take place.
						 * eating loop
						 * Column-major yum yum 
						 */
						int iwas; // iwas is the index. This is how the prof is expected. 
						for(int b = 0; b < numbins;b++){
								fscanf(file,"%d",&iwas);
								for(int c = 0; c < numchans ; c++){
										fscanf(file," %f",&subway[c*numbins + b]);
								}
								fscanf(file,"\n");
						}
				}
				// This function is anyway not called
				void LoadFile(string fn){
						file = fopen(fn.c_str(),"r");
						if(file == NULL) cerr << "PROFIO unable to open file " << fn << endl;
				}
				double getmjd()      { double x = fh->mjd;         return x; }
				double getfract()    { double x = fh->fract;       return x; }
				double getperiod()   { double x = fh->period;      return x; }
				long   getnumpul()   { long   x = fh->numpulses;   return x; }
				float  getfreq()     { float  x = fh->freq;        return x; }
				float  getdm()       { float  x = fh->dm;          return x; }
				int    getnumbins()  { int    x = fh->numbins;     return x; }
				int    getnumchans() { int    x = numchans;        return x; }
				char   gettid()      { char   x = fh->tid;         return x; }
				int    getpol()      { int    x = fh->pol;         return x; }
};

class FITS {
		private:
				fitsfile * fits; // the fitsfile pointer
				int status; // for status, initialized to zero
				PROFIO * ReadThis;
				IOer Pulsar, Observatory, Scan, Project;
				bool configDone;
				double dval;
				float fval;
				int ival;
				long lval;
				char * cpval, date_time[24];
				// The debugging function
				void ReportAndExitOnError(int status){
						if(status) {
								fits_report_error(stderr,status);
								fprintf(stderr, "Error on line %d\n",__LINE__);
								exit(1);
						}
				}
		public:
				FITS(string prof, string pulsar, string observatory, string project, string scan){
						Pulsar.LoadFile(pulsar);
						Scan.LoadFile(scan);
						Observatory.LoadFile(observatory);
						Project.LoadFile(project);
						ReadThis = new PROFIO(prof, stoi(Scan["NUMCHANS"]));
						if(  ReadThis->readHeader() ) {
								cerr << "Unable to read Header\n";
						}; // This is to be the first call.
						configDone = true;
				}
				~FITS() {
						delete ReadThis;
				}
				int sanityCheck() {
						/*
						 * *****************************************************
						 * Post discussion with BCJ, it was decided that parameters from the header will
						 * be given preference over parameters from the cfg files.
						 * This also means we won't be needing any exception handling here.
						 * *****************************************************
						 * By lazy overiding, I mean, the value is not changed in IOer. 
						 * Instead, values will be read from ReadThis only.
						 */
						if(stoi(Scan["NUMCHANS"]) != ReadThis->getnumchans() ) {
								cout << "Overiding NUMCHANS in cfg to what is read from PROF\n";
								// lazy overiding
						}
						if(stoi(Scan["NPOL"]) != ReadThis->getpol() ) {
								cout << "Overiding NPOL in cfg to what is read from PROF\n";
								// lazy overiding
						}
						if(stoi(Pulsar["DM"]) != ReadThis->getdm() ) {
								cout << "Overiding DM in cfg to what is read from PROF\n";
								// lazy overiding
						}
						if(stoi(Project["OBSFREQ"]) != ReadThis->getfreq() ) {
								cout << "Overiding OBSFREQ in cfg to what is read from PROF\n";
						}
						return 0;
				} 
				int createFITS(const char * filename){
						status = 0;
						int bitpix = 8;
						int naxis = 0;
						long * naxes;
						// create
						fits_create_file(&fits, filename, &status);
						// image
						fits_create_img(fits,bitpix,naxis,naxes,&status);
						// first keywords
						fits_update_key(fits, TSTRING, "HDRVER","5.4","Header version",&status);
						fits_update_key(fits,TSTRING,"FITSTYPE","PSRFITS","FITS definition for pulsar data files",&status);
						fits_write_date(fits, &status);
						fits_get_system_time( date_time, &ival, &status );
						return status;
				}
				void configure(string prof, string pulsar, string observatory, string project, string scan){
						Pulsar.LoadFile(pulsar);
						Observatory.LoadFile(observatory);
						Scan.LoadFile(scan);
						Project.LoadFile(project);
						ReadThis->LoadFile(prof);
						configDone = true;
				}
				int closeFITS() { fits_close_file(fits, &status); ReportAndExitOnError(status); return status;}
				int primaryHeader(){
						if(!configDone) { cerr << "Configuration not done..\n"; return -1;}
						cpval = (char*)Project["OBSERVER"].c_str();
						fits_update_key(fits, TSTRING, "OBSERVER", cpval,"Observer name(s)", &status);
						cpval = (char*)Project["PROJID"].c_str();
						fits_update_key(fits, TSTRING, "PROJID", cpval,"Project name", &status);
						cpval = (char*)Observatory["TELESCOP"].c_str();
						fits_update_key(fits, TSTRING, "TELESCOP", cpval,"Telescope name", &status);
						dval = stod(Observatory["ANT_X"]);
						fits_update_key(fits, TDOUBLE, "ANT_X", &dval,"[m] Antenna ITRF X-coordinate (D)", &status);
						dval = stod(Observatory["ANT_Y"]);
						fits_update_key(fits, TDOUBLE, "ANT_Y", &dval,"[m] Antenna ITRF Y-coordinate (D)", &status);
						dval = stod(Observatory["ANT_Z"]);
						fits_update_key(fits, TDOUBLE, "ANT_Z", &dval,"[m] Antenna ITRF Z-coordinate (D)", &status);
						cpval = (char*)Observatory["FRONTEND"].c_str();
						fits_update_key(fits, TSTRING, "FRONTEND", cpval,"Observatory standard name for receiver package", &status);
						ival = stoi(Observatory["IBEAM"]);
						fits_update_key(fits, TINT, "IBEAM",&ival,"Beam ID for multibeam systems", &status);
						ival = stoi(Observatory["NRCVR"]);
						fits_update_key(fits, TINT,"NRCVR",&ival,"Number of receiver polarisation channels", &status);
						cpval = (char*)Observatory["FD_POLN"].c_str();
						fits_update_key(fits, TSTRING, "FD_POLN", cpval,"LIN or CIRC", &status);
						ival = stoi(Observatory["FD_HAND"]);
						fits_update_key(fits, TINT,"FD_HAND",&ival,"+/- 1. +1 is LIN:A=X,B=Y, CIRC:A=L,B=R (I) ", &status);
						fval = stof(Observatory["FD_SANG"]);
						fits_update_key(fits, TFLOAT,"FD_SANG",&fval,"[deg] FA of E vect for equal sig in A&B (E) ", &status);
						fval = stof(Observatory["FD_XYPH"]);
						fits_update_key(fits, TFLOAT, "FD_XYPH", &fval,"[deg] Phase of A* B for injected cal (E) ", &status);
						cpval = (char*)Observatory["BACKEND"].c_str();
						fits_update_key(fits, TSTRING, "BACKEND", cpval,"Backend ID", &status);
						cpval = (char*)Observatory["BECONFIG"].c_str();
						fits_update_key(fits, TSTRING, "BECONFIG", cpval,"Backend configuration file name", &status);
						ival = stoi(Observatory["BE_PHASE"]);
						fits_update_key(fits, TINT, "BE_PHASE", &ival, "0/+1/-1 BE cross-phase:0 unknown,+/-1 std/rev ", &status);
						ival = stoi(Observatory["BE_DCC"]);
						fits_update_key(fits, TINT, "BE_DCC", &ival, "BE downconversion conjugation corrected", &status);
						fval = stof(Observatory["BE_DELAY"]);
						fits_update_key(fits, TFLOAT, "BE_DELAY",&fval, "[s] Backend propn delay from digitiser input ", &status);
						lval = stol(Observatory["TCYCLE"]);
						fits_update_key(fits, TLONG, "TCYCLE", &lval, "Native cycle time of correlation system",&status);
						cpval = (char*)Project["OBS_MODE"].c_str();
						fits_update_key(fits, TSTRING, "OBS_MODE", cpval, "(PSR, CAL, SEARCH) ", &status);
						//cpval = (char*)Scan["DATE-OBS"].c_str();
						ManageMJD mm(ReadThis->getmjd(), ReadThis->getfract());
						string smm = mm.getDateTime();
						cpval = (char*)smm.c_str();
						fits_update_key(fits, TSTRING, "DATE-OBS", cpval ,"UTC date of observation (YYYY-MM-DDThh:mm:ss) ", &status);
						dval = stod(Project["OBSFREQ"]);
						dval = (double)ReadThis->getfreq();
						fits_update_key(fits, TDOUBLE, "OBSFREQ", &dval, "[MHz] Centre frequency for observation",&status);
						dval = stod(Project["OBSBW"]);
						fits_update_key(fits, TDOUBLE, "OBSBW", &dval, "[MHz] Bandwidth for observation ",&status);
						//ival = stoi(Project["OBSNCHAN"]);
						ival = (int)ReadThis->getnumchans();
						fits_update_key(fits, TINT, "OBSNCHAN", &ival, "Number of frequency channels (original) ",&status);
						//fval = stof(Pulsar["DM"]);
						fval = (float)ReadThis->getdm();
						fits_update_key(fits, TFLOAT, "CHAN_DM", &fval, "[cm-3 pc] DM used for on-line dedispersion ",&status);
						cpval = (char*)Scan["PNT_ID"].c_str();
						fits_update_key(fits, TSTRING, "PNT_ID", cpval, "Name or ID for pointing ctr (multibeam feeds) ", &status);
						cpval = (char*)Scan["SRC_NAME"].c_str();
						fits_update_key(fits, TSTRING, "SRC_NAME", cpval, "Source or scan ID ", &status);
						cpval = (char*)Pulsar["COORD_MD"].c_str();
						fits_update_key(fits, TSTRING, "COORD_MD", cpval, "Coordinate mode (J2000, GALACTIC, ECLIPTIC) ",&status);
						fval = stof(Pulsar["EQUINOX"]);
						fits_update_key(fits, TFLOAT, "EQUINOX", &fval, "Equinox of coords (e.g. 2000.0) ",&status);
						cpval = (char*)Pulsar["RA"].c_str();
						fits_update_key(fits, TSTRING, "RA", cpval, "Right ascension (hh:mm:ss.ssss) ",&status);
						cpval = (char*)Pulsar["DEC"].c_str();
						fits_update_key(fits, TSTRING, "DEC", cpval, "Declination (-dd:mm:ss.sss) ", &status);
						// START - Don't know where to take these from 
						fval = stof(Scan["BMAJ"]);
						fits_update_key(fits, TFLOAT, "BMAJ", &fval, "[deg] Beam major axis length ", &status);
						fval = stof(Scan["BMIN"]);
						fits_update_key(fits, TFLOAT, "BMIN", &fval, "[deg] Beam minor axis length ", &status);
						fval = stof(Scan["BPA"]);
						fits_update_key(fits, TFLOAT, "BPA", &fval, "[deg] Beam position angle ", &status);
						cpval = (char*)Scan["STT_CRD1"].c_str();
						fits_update_key(fits, TSTRING, "STT_CRD1", cpval, "Start coord 1 (hh:mm:ss.sss or ddd.ddd) ",&status);
						cpval = (char*)Scan["STT_CRD2"].c_str();
						fits_update_key(fits, TSTRING, "STT_CRD2", cpval, "Start coord 2 (hh:mm:ss.sss or ddd.ddd) ",&status);
						cpval = (char*)Scan["TRK_MODE"].c_str();
						fits_update_key(fits, TSTRING, "TRK_MODE", cpval, "Track mode (TRACK, SCANGC, SCANLAT) ",&status);
						cpval = (char*)Scan["STP_CRD1"].c_str();
						fits_update_key(fits, TSTRING, "STP_CRD1", cpval , "Stop coord 1 (hh:mm:ss.sss or ddd.ddd) ",&status);
						cpval = (char*)Scan["STP_CRD2"].c_str();
						fits_update_key(fits, TSTRING, "STP_CRD2", cpval , "Stop coord 2 (hh:mm:ss.sss or ddd.ddd) ",&status);
						// END
						dval = stod(Scan["SCANLEN"]);
						fits_update_key(fits, TDOUBLE, "SCANLEN", &dval, "[s] Requested scan length (E) ",&status);
						// START - Don't know where to take these from
						cpval =  (char*)Scan["FD_MODE"].c_str();
						fits_update_key(fits, TSTRING, "FD_MODE", cpval, "Feed track mode - FA, CPA, SPA, TPA ",&status);
						fval = stof(Scan["FD_REQ"]);
						fits_update_key(fits, TFLOAT, "FA_REQ", &fval, "[deg] Feed/Posn angle requested (E) ",&status);
						// END
						cpval = (char*)Project["CAL_MODE"].c_str();
						fits_update_key(fits, TSTRING, "CAL_MODE", cpval, "Cal mode (OFF, SYNC, EXT1, EXT2) ",&status);
						dval = stod(Project["CAL_FREQ"]);
						fits_update_key(fits, TDOUBLE, "CAL_FREQ", &dval, "[Hz] Cal modulation frequency (E) ",&status);
						fval = stof(Project["CAL_DCYC"]);
						fits_update_key(fits, TFLOAT, "CAL_DCYC", &fval, "Cal duty cycle (E) ",&status);
						fval = stof(Project["CAL_PHS"]);
						fits_update_key(fits, TFLOAT, "CAL_PHS", &fval, "Cal phase (wrt start time) (E) ",&status);
						ival = stoi(Project["CAL_NPHS"]);
						fits_update_key(fits, TINT, "CAL_NPHS", &ival, "Number of states in cal pulse (I) ",&status);
						//lval = stol(Scan["STT_IMJD"]);
						lval = (long) ReadThis->getmjd();
						fits_update_key(fits, TLONG, "STT_IMJD", &lval, "Start MJD (UTC days) (J - long integer)",&status);
						//lval = stol(Scan["STT_SMJD"]);
						lval = (long) ReadThis->getfract();
						fits_update_key(fits, TLONG, "STT_SMJD", &lval, "[s] Start time (sec past UTC 00h) (J) ",&status);
						//dval = stod(Scan["STT_OFFS"]);
						dval = ReadThis->getfract() - lval;
						fits_update_key(fits, TDOUBLE, "STT_OFFS", &dval,"[s] Start time offset (D) ",&status);
						//dval = stod(Scan["STT_LST"]); // CHECK THIS :
						dval = ReadThis->getfract();
						fits_update_key(fits, TDOUBLE, "STT_LST", &dval, "[s] Start LST (D) ",&status);
						//ival = stoi(Scan["NPOL"]);
						ival = ReadThis->getpol();
						fits_update_key(fits, TINT, "NPOL", &ival, "Number of Polarisations(I)", &status);
						return status;
				}
				int PutHistoryTable() {
						if(!configDone) { cerr << "Configuration not done..\n"; return -1;}
						/*
						   Missed the following here :
						   fd_corr, be_corr, rm_model, aux_rm_c, dm_model, aux_dm_c
						   When writing TSTRING to fits_write_col you gotta send pointer to char*.
						   */
						int col = 1, nrows = 0, ncols = 18, n = 0;
						char *PHtype[20] = {
								"DATE_PRO","PROC_CMD","POL_TYPE","NPOL    ","NBIN    ","NBIN_PRD","TBIN    ","CTR_FREQ",
								"NCHAN   ","CHAN_BW ","PAR_CORR","RM_CORR ","DEDISP  ","DDS_MTHD","SC_MTHD ","CAL_MTHD",
								"CAL_FILE","RFI_MTHD" // ,"SCALE","NSUB"
						};
						char *PHform[20] = {
								"24A     ","80A     ","8A      ","1I      ","1I      ","1I      ","1D      ","1D      ",
								"1I      ","1D      ","1I      ","1I      ","1I      ","32A     ","32A     ","32A     ",
								"32A     ","32A     " // ,"8A      ","1I      "
						};
						char *PHunit[20] = {
								"        ","        ","        ","        ","        ","        ","s       ","MHz     ",
								"        ","MHz     ","        ","        ","        ","        ","        ","        ",
								"        ","        " // ,"        ","        "
						};
						fits_create_tbl( fits, BINARY_TBL, nrows, ncols, PHtype, PHform, PHunit, "HISTORY", &status);
						/* Processing date and time (YYYY-MM-DDThh:mm:ss UTC) */
						cpval = date_time;
						fits_write_col( fits, TSTRING, 1, 1, 1, 1, &cpval, &status );
						/* Processing program and command */
						cpval = (char*)Observatory["PROC_CMD"].c_str();
						fits_write_col( fits, TSTRING, 2, 1, 1, 1, &cpval, &status );
						/* Polarisation identifier */
						cpval = (char*)Scan["POL_TYPE"].c_str();
						fits_write_col( fits, TSTRING, 3, 1, 1, 1, &cpval, &status );
						/* Nr of pols*/
						//ival = stoi(Scan["NPOL"]);
						ival = ReadThis->getpol();
						fits_write_col( fits, TSHORT, 4, 1, 1, 1, &ival, &status );
						/* Nr of bins per product (0 for SEARCH mode) */
						ival = ReadThis->getnumbins();
						//ival = stoi(Scan["NUMBINS"]);
						fits_write_col( fits, TSHORT, 5, 1, 1, 1, &ival, &status );
						/* Nr of bins per period */
						// TODO: Why is same thing written again? check
						// # of bins per period is numbins, that's how you define it
						fits_write_col( fits, TSHORT, 6, 1, 1, 1, &ival, &status );
						/* Bin time */
						dval = (double) ReadThis->getperiod() / ReadThis->getnumbins();
						fits_write_col( fits, TDOUBLE, 7, 1, 1, 1, &dval, &status );
						/* Centre freq. */
						// dx = (double)(fh->freq);
						//dval = stod(Project["OBSFREQ"]);
						dval = (double)ReadThis->getfreq();
						fits_write_col( fits, TDOUBLE, 8, 1, 1, 1, &dval, &status );
						/* Number of channels */
						//ival = stoi(Scan["NUMCHANS"]);
						ival = (int)ReadThis->getnumchans();
						fits_write_col( fits, TSHORT, 9, 1, 1, 1, &ival, &status );
						/* Channel bandwidth */
						dval = - (double) stod(Project["OBSBW"]) / ival; 
						// printf("Channel Bandwidth %lf\n", dx);
						fits_write_col( fits, TDOUBLE, 10, 1, 1, 1, &dval, &status );
						ival = 0;
						/* Parallactic angle correction applied */
						fits_write_col( fits, TSHORT, 11, 1, 1, 1, &ival, &status );
						/* RM correction applied */
						fits_write_col( fits, TSHORT, 12, 1, 1, 1, &ival, &status );
						/* Data dedispersed */
						ival = 1;
						fits_write_col( fits, TSHORT, 13, 1, 1, 1, &ival, &status );
						/* Dedispersion method */
						cpval = (char*)Scan["DDS_MTHD"].c_str();
						fits_write_col( fits, TSTRING, 14, 1, 1, 1, &cpval, &status );
						/* Scattered power correction method */
						cpval = (char*)Scan["SC_MTHD"].c_str();
						fits_write_col( fits, TSTRING, 15, 1, 1, 1, &cpval, &status );
						/* Calibration method */
						cpval = (char*)Scan["CAL_MTHD"].c_str();
						fits_write_col( fits, TSTRING, 16, 1, 1, 1, &cpval, &status );
						/* Name of calibration file */
						cpval = (char*)Scan["CAL_FILE"].c_str();
						fits_write_col( fits, TSTRING, 17, 1, 1, 1, &cpval, &status );
						/* RFI excision method */
						cpval = (char*)Scan["RFI_MTHD"].c_str();
						fits_write_col( fits, TSTRING, 18, 1, 1, 1, &cpval, &status );
						// /* SCALE */
						// cpval = "FluxDen";
						// fits_write_col(fits, TSTRING, 19, 1, 1, 1, &cpval, &status);
						// /* NSUB */
						// printf("NSUBS zero ? %d \n", fh->numBins);
						// fits_write_col(fits, TINT, 20, 1,1,1, &(fh->numBins), &status);
						return(status);
				}
				int PutSubint(){
						// int sta;
						int nscl, ndata;
						nscl = ReadThis->getnumchans() * ReadThis->getpol();   // fh->numChan * fh->pol;
						ndata = ReadThis->getnumbins() * nscl; //  fh->numBins * nscl;
						int i,j, k; // These are my iterators ?
						/*
						 *float x;
						 *double dx, dy, dz, ds, dc;
						 */
						int ncols=21, col;
						char *ttype[ncols], *tform[ncols], *tunit[ncols];
						long naxes[3];

						char Cstr16[16], Estr16[16], Istr16[16];

						/* Create SUBINT BINTABLE */
						long long nrows = 0; /* naxis2 - Let CFITSIO sort this out */
						/*
						   Why keep a standard when you don't follow it?
						   Adding a period field in the SUBINT BINTABLE
						*/

						ttype[0] = "ISUBINT ";    /* Subint number. If NAXIS=-1, 0 indicates EOD. */
						tform[0] = "1J      ";
						tunit[0] = "";
						ttype[1] = "INDEXVAL";    /* Optionally used if INT_TYPE != TIME */
						tform[1] = "1D      ";
						tunit[1] = "";
						ttype[2] = "TSUBINT ";    /* [s] Length of subintegration */
						tform[2] = "1D      ";
						tunit[2] = "";
						ttype[3] = "OFFS_SUB";    /* [s] Offset from Start UTC of subint centre */
						tform[3] = "1D      ";
						tunit[3] = "";
						ttype[4] = "LST_SUB ";    /* [s] LST at subint centre */
						tform[4] = "1D      ";
						tunit[4] = "";
						ttype[5] = "RA_SUB  ";    /* [turns] RA (J2000) at subint centre */
						tform[5] = "1D      ";
						tunit[5] = "";
						ttype[6] = "DEC_SUB ";    /* [turns] Dec (J2000) at subint centre */
						tform[6] = "1D      ";
						tunit[6] = "";
						ttype[7] = "GLON_SUB";    /* [deg] Gal longitude at subint centre */
						tform[7] = "1D      ";
						tunit[7] = "";
						ttype[8] = "GLAT_SUB";    /* [deg] Gal latitude at subint centre */
						tform[8] = "1D      ";
						tunit[8] = "";
						ttype[9] = "FD_ANG  ";    /* [deg] Feed angle at subint centre */
						tform[9] = "1E      ";
						tunit[9] = "";
						ttype[10] = "POS_ANG ";    /* [deg] Position angle of feed at subint centre */
						tform[10] = "1E      ";
						tunit[10] = "";
						ttype[11] = "PAR_ANG ";    /* [deg] Parallactic angle at subint centre */
						tform[11] = "1E      ";
						tunit[11] = "";
						ttype[12] = "TEL_AZ  ";    /* [deg] Telescope azimuth at subint centre */
						tform[12] = "1E      ";
						tunit[12] = "";
						ttype[13] = "TEL_ZEN ";    /* [deg] Telescope zenith angle at subint centre */
						tform[13] = "1E      ";
						tunit[13] = "";

						sprintf( Cstr16, "%dD", ReadThis->getnumchans() );
						ttype[14] = "DAT_FREQ";
						tform[14] = Cstr16;
						tunit[14] = "";
						ttype[15] = "DAT_WTS ";
						tform[15] = Cstr16;
						tunit[15] = "";

						sprintf( Estr16, "%dE", nscl );
						ttype[16] = "DAT_OFFS";
						tform[16] = Estr16;
						tunit[16] = "";
						ttype[17] = "DAT_SCL ";
						tform[17] = Estr16;
						tunit[17] = "";

						sprintf( Istr16, "%dE", ndata );
						ttype[18] = "DATA    ";
						tform[18] = Istr16;
						tunit[18] = "Jy      ";

						// Y keep a standard if you don't follow it?
						ttype[19] = "PERIOD  ";
						tform[19] = "1D      ";
						tunit[19] = "s       ";
						// Y seriously?
						ttype[20] = "NPOL    ";
						tform[20] = "1I      ";
						tunit[20] = "";
						// Table
						fits_create_tbl( fits, BINARY_TBL, nrows, ncols, ttype, tform, tunit, "SUBINT", &status);
						ReportAndExitOnError(status);
						/* Add dimensions of column 'ncols' = SUBINT Data */
						naxes[0] = ReadThis->getnumbins();// fh->numBins;
						naxes[1] = ReadThis->getnumchans();// fh->numChan;
						naxes[2] = ReadThis->getpol();
						// write tdim 
						fits_write_tdim( fits, 19, 3, naxes, &status );
						ReportAndExitOnError(status);

						/* Add keywords */
						cpval = (char*)Scan["INT_TYPE"].c_str();
						fits_update_key( fits, TSTRING, "INT_TYPE", cpval, "Time axis (TIME, BINPHSPERI, BINLNGASC, etc)", &status);
						cpval = (char*)Scan["INT_UNIT"].c_str();
						fits_update_key( fits, TSTRING, "INT_UNIT", "SEC", "Unit of time axis (SEC, PHS (0-1), DEG)", &status);
						cpval = (char*)Scan["SCALE"].c_str();
						fits_update_key(fits, TSTRING, "SCALE", "FluxDen","Intensity units (FluxDen/RefFlux/Jansky) ",&status);
						cpval = (char*)Scan["POL_TYPE"].c_str();
						fits_update_key(fits, TSTRING, "POL_TYPE", "AA", "Polarisation identifier (e.g., AABBCRCI, AA+BB)", &status);
						ival = ReadThis->getpol();
						fits_update_key(fits, TINT, "NPOL", &ival, "Nr of polarisations ",&status);
						fval = (float)ReadThis->getperiod() / ReadThis->getnumbins();
						fits_update_key(fits, TFLOAT, "TBIN", &fval, "[s] Time per bin or sample ", &status);
						ival = stoi(Observatory["NBITS"]);
						fits_update_key(fits, TINT, "NBITS", &ival," Number of bits per sample", &status);
						ival = ReadThis->getnumbins();
						fits_update_key(fits, TINT, "NBIN", &ival, "Nr of bins (PSR/CAL mode; else 1) ", &status);
						ival = ReadThis->getnumchans();
						fits_update_key(fits, TINT, "NCHAN", &ival, "Number of channels/sub-bands in this file ", &status);
						dval = (double) stod(Project["OBSBW"])  / ReadThis->getnumchans();
						fits_update_key(fits, TDOUBLE,"CHAN_BW",&dval,"[MHz] Channel/sub-band width ", &status);
						fval = (float) ReadThis->getdm();
						fits_update_key(fits, TFLOAT, "DM", &fval, "[cm-3 pc] DM for post-detection dedisperion ", &status);
						fval = stof(Pulsar["RM"]);
						fits_update_key(fits, TFLOAT, "RM", &fval, "[rad m-2] RM for post-detection deFaraday", &status);
						fval = stof(Scan["NCHNOFFS"]);
						fits_update_key(fits, TFLOAT, "NCHNOFFS", &fval, "Channel/sub-band offset for split files ", &status);
						ival = stoi(Project["NSBLK"]); // This has to be default of one
						fits_update_key(fits, TINT, "NSBLK", &ival, "Samples/row (SEARCH mode, else 1)", &status);
						ival = stoi(Project["NSTOT"]); // This has to be default of one
						fits_update_key(fits, TINT, "NSTOT", &ival, "Total number of samples (SEARCH mode, else 1) ", &status);
						ReportAndExitOnError(status);
						// Now focusing on writing the subintegration data.
						// Declaring stuff here
						// Such bad programming practises 
						// Much wow
						int subint_cnt = 1;
						float *binned_weight, *binned_offset, *binned_scale;
						float *binned_freq;
						float * subs;
						while(ReadThis->MoreThere()){
								/*
								 *There is a major assumption happening here.
								 *It is assumed that First Header of the prof has been read. 
								 */
								nscl  = ReadThis->getnumchans() * ReadThis->getpol();// fh->numChan * fh->pol;
								ndata = ReadThis->getnumbins() * nscl; // fh->numBins * nscl;
								subs  = new float[ndata];

								binned_weight = new float[ReadThis->getnumchans()] ;
								binned_offset = new float[nscl];
								binned_scale  = new float[nscl];
								
								ReadThis->readSubs(subs);
								/*
								 *binned_weight = (float*)malloc(ReadThis->getnumchans() * sizeof(float));   // (float*)malloc(fh->numChan*sizeof(float));
								 *binned_offset = (float*)malloc(nscl*sizeof(float));
								 *binned_scale = (float*)malloc(nscl*sizeof(float));
								 */
								for(j = 0; j < ReadThis->getnumchans();j++){ // numchans
										binned_weight[j] = 1.0;
								}
								for(j=0;j<nscl;j++){
										binned_offset[j] = 0.0;
										binned_scale[j] = 1.0;
								}
								fval = stof(Project["OBSBW"]) / ReadThis->getnumchans();
								//float dx = ph->obsbw / fh->numChan;
								binned_freq = (float*)malloc(ReadThis->getnumchans() * sizeof(float));
								int ni, nc = ReadThis->getnumchans() / 2; // numChan is always a power of 2
								for(ni = 0; ni < ReadThis->getnumchans(); ni++){
										binned_freq[ni] = (float) ReadThis->getfreq() - ((float) ni * fval);
								}
								col =1;
								/* Subint number. If NAXIS=-1, 0 indicates EOD. */
								fits_write_col( fits, TINT, col, subint_cnt, 1, 1, &subint_cnt, &status );
								col++;
								/* INDEXVAL - Optionally used if INT_TYPE != TIME */
								dval = 0.0;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;
								/* [s] Length of subint ALAKAZAM */
								dval = (double) ReadThis->getperiod() * (double) ReadThis->getnumpul();
								//double mdx = fh->period * (double)fh->numPulses;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;
								/* [s] Offset from Start UTC of subint centre */
								// dx = sum_subint_mid_pt / slen;
								// dx = 86400.0*fh[i]->fract/(2*dx);
								// printf("This is what ? %lf",dx);
								//dx = f->fract/2;
								/************************
								 * I'm checking if I what I have coded above is actually correct or not
								 * dx = fh->fract/2; # This is what I had above 
								 * I am chaning that to fh->fract + slen/2 
								 * Intuition wise, it makes sense to me.
								 * **********************
								 * Actually testing out with ORT data in the 
								 * 1st busy week made me realize what I had is actually work
								 * reverting back to the original formula
								 * **********************
								 * Understanding what the formula says
								 * fh->fract*slen/(mid*2)
								 * **********************
								 * After hours of debugging,
								 * I am going to set this to zero 
								 * *********************/
								//dval = ReadThis->getfract() / 2;
								dval = 0.00;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								/* [s] LST at subint centre */
								// ds = sum_subint_lst_sin / sum_subint_len_secs;
								// dc = sum_subint_lst_cos / sum_subint_len_secs;
								// if( ( dx = ( atan2( ds, dc ) / TwoPi ) ) < 0.0 ) dx += 1.0;
								//dval = 86400.0 * (double)ReadThis->getfract();
								//dx = 86400.0*fh->fract;
								dval = 0.00;	
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								// /* [turns] RA (J2000) at subint centre */
								// ds = sum_subint_ra_sin / sum_subint_len_secs;
								// dc = sum_subint_ra_cos / sum_subint_len_secs;
								// if( ( dx = ( atan2( ds, dc ) / TwoPi ) ) < 0.0 ) dx += 1.0;
								// dx = 1234.1234;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								/* [turns] Dec (J2000) at subint centre */
								// ds = sum_subint_dec_sin / sum_subint_len_secs;
								// dc = sum_subint_dec_cos / sum_subint_len_secs;
								// dx = atan2( ds, dc ) / TwoPi;
								// dx = 1234.1234;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								/* [deg] Gal longitude at subint centre */
								// ds = sum_subint_Glon_sin / sum_subint_len_secs;
								// dc = sum_subint_Glon_cos / sum_subint_len_secs;
								// if( ( dx = ( atan2( ds, dc ) / TwoPi ) ) < 0.0 ) dx += 1.0;
								//dval = 360.0;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								/* [deg] Gal latitude at subint centre */
								// ds = sum_subint_Glat_sin / sum_subint_len_secs;
								// dc = sum_subint_Glat_cos / sum_subint_len_secs;
								// dx = atan2( ds, dc ) * 360.0 / TwoPi;
								// dx = 360.0;
								fits_write_col( fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status );
								col++;

								/* [deg] Feed angle at subint centre */
								// ds = sum_subint_fa_sin / sum_subint_len_secs;
								// dc = sum_subint_fa_cos / sum_subint_len_secs;
								// dx = atan2( ds, dc ) * 360.0 / TwoPi;
								fval = 360.0;
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, 1, &fval, &status );
								col++;

								/* [deg] Parallactic angle at subint centre */
								// ds = sum_subint_pa_sin / sum_subint_len_secs;
								// dc = sum_subint_pa_cos / sum_subint_len_secs;
								// dy = atan2( ds, dc ) * 360.0 / TwoPi;

								/* [deg] Position angle of feed at subint centre */
								// dx = dx + dy;
								// if( dx > 180.0 ) dx -= 360.0;
								// if( dx < 180.0 ) dx += 360.0;
								// x = (float) dx;
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, 1, &fval, &status );
								col++;

								/* [deg] Parallactic angle at subint centre */
								// x = (float) d;
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, 1, &fval, &status );
								col++;

								/* [deg] Telescope azimuth at subint centre */
								// ds = sum_subint_az_sin / sum_subint_len_secs;
								// dc = sum_subint_az_cos / sum_subint_len_secs;
								// if( ( dx = ( atan2( ds, dc ) / TwoPi ) ) < 0.0 ) dx += 1.0;
								// dx = 360.0;
								//fval = (float) dval;
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, 1, &fval, &status );
								col++;

								/* [deg] Telescope zenith angle at subint centre */
								// ds = sum_subint_el_sin / sum_subint_len_secs;
								// dc = sum_subint_el_cos / sum_subint_len_secs;
								// dx = 90.0 - ( atan2( ds, dc ) * 360.0 / TwoPi );
								// x = (float) dx;
								// x  = 360.0;
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, 1, &fval, &status );
								col++;
								/* Centre freq. for each channel - NCHAN floats */
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, ReadThis->getnumchans(), binned_freq, &status );
								col++;
								/* Weights for each channel -  NCHAN floats */
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, ReadThis->getnumchans(), binned_weight, &status );
								col++;
								/* Data offset for each channel - NCHAN*NPOL floats */
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, nscl, binned_offset, &status );
								col++;
								/* Data scale factor for each channel - NCHAN*NPOL floats */
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, nscl, binned_scale, &status );
								col++;
								/* Subint data table - Dimensions of data table = (NBIN,NCHAN,NPOL) */
								fits_write_col( fits, TFLOAT, col, subint_cnt, 1, ndata, subs, &status );
								col++;
								ReportAndExitOnError(status);
								/* Why keep a standard when you have to break it? */
								dval = ReadThis->getperiod();
								fits_write_col(fits, TDOUBLE, col, subint_cnt, 1, 1, &dval, &status);
								col++;
								/* Why keep a standard when you have to break it? */
								ival = ReadThis->getpol();
								fits_write_col(fits, TINT, col, subint_cnt, 1, 1, &ival, &status);
								col++;
								subint_cnt++;
								delete[] binned_weight;
								delete[] binned_offset;
								delete[] binned_scale;
								delete[] binned_freq;
								delete[] subs;
								/*
								 *free(binned_weight);free(binned_offset);free(binned_scale);
								 *free(binned_freq);
								 *free(fh);
								 *free(subs);
								 */
								ReadThis->readHeader(); // ensure fh->numChan is loaded
								//dflag++;
								/* Finished SUBINT */
						}
						ReportAndExitOnError(status);
						return status;
				}
};
