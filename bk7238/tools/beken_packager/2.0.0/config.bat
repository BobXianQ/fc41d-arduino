@echo off


set bootpath=%1
set apppath=%2

echo	{																	>  %3/config.json
echo	 	"count": 2,                                                     >> %3/config.json
echo	 	"magic": "RT-Thread",                                           >> %3/config.json
echo		"section": [                                                    >> %3/config.json
echo		{                                                               >> %3/config.json
echo			"firmware": "%bootpath:\=\\%",          					>> %3/config.json
echo			"partition": "bootloader",                                  >> %3/config.json
echo			"size": "65280",                                            >> %3/config.json
echo			"start_addr": "0x00000000",                                 >> %3/config.json
echo			"version": "2M.1220"                                        >> %3/config.json
echo		},                                                              >> %3/config.json
echo		{                                                               >> %3/config.json
echo			"firmware": "%apppath:\=\\%",                     			>> %3/config.json
echo			"partition": "app",                                         >> %3/config.json
echo			"size": "1156K",                                            >> %3/config.json
echo			"start_addr": "0x00011000",                                 >> %3/config.json
echo			"version": "2M.1220"                                        >> %3/config.json
echo		}                                                               >> %3/config.json
echo		],                                                              >> %3/config.json
echo		"version": "0.1"                                                >> %3/config.json
echo	}                                                                   >> %3/config.json

