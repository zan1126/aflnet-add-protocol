//add NGTCP in the main function
case 'P': 
	     if (!strcmp(optarg, "NGTCP")) {
          extract_requests = &extract_requests_quic;
          extract_response_codes = &extract_response_codes_quic;
	     } else {
          FATAL("%s protocol is not supported yet!", optarg);
       }
