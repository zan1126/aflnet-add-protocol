//add NGTCP in the main function
case 'P': 
	     if (!strcmp(optarg, "NGTCP")) {
          extract_requests = &extract_requests_ngtcp;
          extract_response_codes = &extract_response_codes_ngtcp;
	     } else {
          FATAL("%s protocol is not supported yet!", optarg);
       }
