
-- Export of NMEA sentences to be used with any application or device capable of using them.
-- Uncomment only one of the two lines below corresponding to your DCS Saved Games path. You might need to edit the path if different than any of them.
userprofile = os.getenv ("userprofile"):gsub("\\","/")
-- dofile(userprofile .. "/Saved Games/DCS/Scripts/DCS_NMEA.lua")
dofile(userprofile .. "/Saved Games/DCS.openbeta/Scripts/DCS_NMEA.lua")

pcall(function() local dcsSr=require('lfs');dofile(dcsSr.writedir()..[[Mods\Services\DCS-SRS\Scripts\DCS-SimpleRadioStandalone.lua]]); end,nil);

local Tacviewlfs=require('lfs');dofile(Tacviewlfs.writedir()..'Scripts/TacviewGameExport.lua')
