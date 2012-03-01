/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "http_connection.h"

#include "apr_strings.h"

#include <stdio.h>
#include <sys/stat.h>


char static *source = NULL;

int file_exists(const char * filename)
{
    FILE * file = fopen(filename, "r");
    if (file != NULL)
    {
        fclose(file);
        return 1;
    }
    return 0;
}


int read_file(const char * filename){
	long bufsize;
	int count;
	int error = 0;
	FILE * fp = fopen(filename, "r");
	if (fp != NULL) {
		if (fseek(fp, 0L, SEEK_END) == 0) {
			bufsize = ftell(fp);
			if (bufsize == -1) 
				error = -1 ;
	
			source = malloc(sizeof(char) * (bufsize + 1));

			if (fseek(fp, 0L, SEEK_SET) == 0) 
				error = -1; 

			size_t newLen = fread(source, sizeof(char), bufsize, fp);

			if (newLen == 0) {
				error = -1;
			} else {
				source[++newLen] = '\0';
			}
		}

	} 
	fclose(fp);

	if(error == -1) return 0;
	return 1;
}


static int less_handler(request_rec* r)
{
	struct stat info;
	struct stat lessinfo;
	char * lessfilename = NULL;
	char * command = NULL;
	int status;
	int newer = 0;
	int error = 0;
	char * errornum = NULL;

	if (!r->handler || strcmp(r->handler, "less"))
	return DECLINED;

	if (r->method_number != M_GET)
	return HTTP_METHOD_NOT_ALLOWED;

	asprintf(&lessfilename, "%s.less", r->filename);
	asprintf(&command, "lessc %s > %s", r->filename, lessfilename);

	if (!file_exists(lessfilename)){

		status = system(command);
		if(status != 0)
			return DECLINED;

	}

	if (file_exists(lessfilename)){

		if ((stat(r->filename,&info) == 0) && (stat(lessfilename,&lessinfo) == 0) && (info.st_mtime > lessinfo.st_mtime))
			status = system(command);
		
		if ((error = read_file(lessfilename)) != 1){
			ap_set_content_type(r, "text/css");
        		ap_rputs(source,r);
			free(source);
			return OK;

		} else {

			asprintf(&errornum, "%d", error);
			ap_set_content_type(r, "text/css");
        		ap_rputs(errornum,r);
			free(source);
			return OK;

		}

	}
    	
}
 
static void register_hooks(apr_pool_t* pool)
{
    ap_hook_handler(less_handler, NULL, NULL, APR_HOOK_MIDDLE);
}
 
module AP_MODULE_DECLARE_DATA less_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    register_hooks
};
