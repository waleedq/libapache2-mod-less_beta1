# mod-less is LESS apache2 module :

This apache2 module is written to compile LESS code inside the css files when they are requested so the process will 
be transparent from any script using css no matter what it's language is

this apache2 module is written using c.

# How it works :

when a request is sent to any css file this request will be handeled by this module, the module will compile the css file and create another file from this css file with .less extension, then the module will pass the compiled version as a resposne to the request 

example:

		 request: http://localhost/themes/css/style.css <=== this css file containes LESS codes insde it

		  the module will compile style.css and save the compiled version to :
		 
		 resposne: http://localhost/themes/css/style.css.less <==== this is the compiled version from style.css
		  
		  then the moduel will return the compiled version as response for the file "style.css"

# Dependencies : 

	Apache2
		sudo apt-get install apache2

	Less Css 
		
		sudo apt-get install rubygems1.8
		sudo gem install rubygems-update
		sudo update_rubygems     
		sudo gem install less

# Installation :
	
	copy /bin/mod_less.so to /usr/lib/apache2/modules/mod_less.so
		  chmod 644 /usr/lib/apache2/modules/mod_less.so
	
	copy /bin/less.load to /etc/apache2/mods-available/less.load
	copy /bin/less.conf to /etc/apache2/mods-available/less.conf

you can edit the less.conf file to change when this module is gonna be invoked 

	for example : this conf with "\.css$" will invoke the module when any .css file is requested 
		<FilesMatch "\.css$">
		   SetHandler less
		</FilesMatch>

	changing "\.css$" to "\.less$" will invoke the module only for requested files with .less extension

# Compile from source :
source can be found in /src/mod_less.c
to compile it you will need the apxs2 tool which can be found in the apache2-prefork-dev package

	sudo apt-get install apache2-prefork-dev

after installing apxs2 tool, cd to the src directory and execute this command 

	sudo apxs2 -c -i mod_less.c

compiled file "mod_less.so" will be automatically copied to /usr/lib/apache2/modules/mod_less.so and chmoded.
you only need to add the less.load and less.conf as mentioned in the Installation section above.

# Developers :

waleed al qadi (waleedq).
	
	note: feel free to fork and contrib to this project
		
	
