/**
 * @Author: Daniel Murrieta M.Sc. Student FH Aachen - CIDESI <daniel>
 * @Date:   2020-05-02T23:38:27+02:00
 * @Email:  daniel.murrieta-alvarez@alumni.fh-aachen.de
 * @Filename: WeatherStat_Messages.h
 * @Last modified by:   daniel
 * @Last modified time: 2021-05-19T13:54:07+02:00
 * @License: CC by-sa
 */

 #ifndef WeatherStat_Messages_h
 #define WeatherStat_Messages_h
 String StartJS = "[\"";
 String SpacerJS = "\",\"";
 String StopJS = "\"]";

  char *messages_quality[] = {
   "Excellent",
   "Good",
   "Lightly polluted",
   "Moderately polluted",
   "Heavily polluted",
   "Severely polluted",
   "Extremely polluted"
 };

  char *messages_impact [] = {
   "Best for well-being",
   "No impact on well-being",
   "Reduction of well-being possible",
   "More significant irritation possible",
   "Exposition might lead to effects like headache",
   "More severe health issue possible if harmful VOC present",
   "Headaches, additional neurotoxic effects possible"
 };
 //
 char *messages_saction [] = {
   "No measures needed",
   "No measures needed",
   "Ventilation suggested",
   "Increase ventilation with clean air",
   "Optimize ventilation",
   "Contamination should be identified, maximize ventilation and reduce attendance",
   "Contamination needs to be identified, avoid presence and maximize ventilation"
 };
 //
  char *messages_iaqcolors [] = {
   "#80E335",
   "#A5CF5B",
   "#FEFB3B",
   "#E37D1D",
   "#DC2A04",
   "#831D4C",
   "#5B3606"
 };
//  char *messages_iaqcolors [] = {
//   "80E335",
//   "A5CF5B",
//   "FEFB3B",
//   "E37D1D",
//   "DC2A04",
//   "831D4C",
//   "5B3606"
// };
 //
 char *messages_accuracy [] = {
   "Stabilization, run-in ongoing",
   "Low accuracy, auto-trimming on-going",
   "Medium accuracy, auto-trimming ongoing",
   "High accuracy"
 };

 char *messages_runin_stat[] = {
   " Power-on stabilization ongoing",
   " Power-on stabilization finished"
 };

 char *messages_stab_stat[] = {
   "Initial sensor stabilization ongoing",
   "Initial sensor stabilization finished"
 };

  uint8_t iaq_Index2Level(int index);
 #endif   // that goes allways it closes the library
