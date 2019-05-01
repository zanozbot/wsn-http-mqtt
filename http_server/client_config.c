// AWS IoT client endpoint
const char *client_endpoint = "a1n8w57lqdrsd4.iot.us-east-1.amazonaws.com";

// AWS IoT device certificate (ECC)
const char *client_cert =
"-----BEGIN CERTIFICATE-----\r\n"
"MIICtTCCAZ2gAwIBAgIUW5S9IX1ZZK6eyy/W3cBKJtfpXZEwDQYJKoZIhvcNAQEL\r\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\r\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDQyODA5MjY1\r\n"
"M1oXDTQ5MTIzMTIzNTk1OVowRTELMAkGA1UEBhMCU0kxEzARBgNVBAgMClNvbWUt\r\n"
"U3RhdGUxITAfBgNVBAoMGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDBZMBMGByqG\r\n"
"SM49AgEGCCqGSM49AwEHA0IABNopl8v5TbRNWqh5ImWgd55/YiCErIwqbry8W0vc\r\n"
"4rQccC94IXiqDJmIMz/UMLAgd0xNUKjQm4768gExUQ9fl/GjYDBeMB8GA1UdIwQY\r\n"
"MBaAFNOokiATV2W03iIvzpwSGbEIUa+RMB0GA1UdDgQWBBTOOdcSTCc+VSR8B7l3\r\n"
"AuzRBMrg7TAMBgNVHRMBAf8EAjAAMA4GA1UdDwEB/wQEAwIHgDANBgkqhkiG9w0B\r\n"
"AQsFAAOCAQEAABPZhxPvQiiV6wmqv7+lPditJmbmmSCUwnfCCNdp2ErvDrPd94eg\r\n"
"zDIyUZU5GpB50Sbf/eVh915jQYdUvvS5P61Jf14FDK7DyfRGOdV6Jy2Hd8yXG2eu\r\n"
"N1kzwW2Go12U2Uj27T9nkUtAFTZZaYVi2GX8L5l+LuZvYDFf2oK+QDUbBWaFTS3+\r\n"
"QgtK9NE0V7YO9W/gD4oWl0dOkQw1LMNqVU/IYnYHEdErSq1sZAlimLVqrc3vETfZ\r\n"
"3+1TNiNwWIfM02nSpN8iHQLAIEQSalCWgE942GQJD16ZHF11frGHFJlrM5wQ9Xbn\r\n"
"m+JjkXl6AXOatSbXXolvgqLKUiyON3aAJw==\r\n"
"-----END CERTIFICATE-----\r\n";

// AWS IoT device private key (ECC)
const char *client_key = 
"-----BEGIN EC PARAMETERS-----\r\n"
"BggqhkjOPQMBBw==\r\n"
"-----END EC PARAMETERS-----\r\n"
"-----BEGIN EC PRIVATE KEY-----\r\n"
"MHcCAQEEIPviv3NnwzYzUHrugtmAd7k2cAGVL6iewyttf0DsaeNjoAoGCCqGSM49\r\n"
"AwEHoUQDQgAE2imXy/lNtE1aqHkiZaB3nn9iIISsjCpuvLxbS9zitBxwL3gheKoM\r\n"
"mYgzP9QwsCB3TE1QqNCbjvryATFRD1+X8Q==\r\n"
"-----END EC PRIVATE KEY-----\r\n";
