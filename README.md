# Multithreaded Webcrawler

## Description
Multithreaded webcrawler written in C++. It requires url of page to start crawling, regular expression of strings to search for, depth of recursion in which to look for strings defined by regular expression and number of threads to use.

## Dependencies
```
libcurl
C++ in version at least 14
```

## Compile
```
cd MultithreadedWebcrawler
mkdir build
cd build
cmake ..
make
```

## Config file
```
url_regex=(http|ftp|https)://([\w_-]+(?:(?:\.[\w_-]+)+))([\w.,@?^=%&:/~+#-]*[\w@?^=%&/~+#-])?
regex=[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+
start_url=https://panoramafirm.pl/szukaj?k=lekarz&l=warszawa
max_threads=4
max_depth=2
```

## Using
```
./MultithreadedWebcrawler [path_to_config_file]
    [path_to_config_file] - relative path to config file
```

## Example
```
./MultithreadedWebcrawler ../config.cfg
```

## Output
```
jziolkow@wp.pl
malgosia.pietowska@wp.pl
kontak@koronyporcelanowe.pl
kami.lecznica@poczta.internetdsl.pl
radoslaw1983@gmail.com
kalinalap@tlen.pl
cmp@hipokrates.tychy.pl
recepcja@przychodniaeumedica.pl
sanus.kalisz@gmail.com
katarzyna.pietrzak57@wp.pl
jola_piotrowicz@autograf.pl
rojecka@wp.pl
medjana@medjana.pl
kasiaflak@o2.pl
gabinet@drwaclawik.pl
kancelaria@achler.pl
odl@fundacja.walbrzych.pl
achtamalka@o2.pl
krysia@cmeureka.pl
michalak.k12@gmail.com
angrzel@gazeta.pl
```

## Author
Bartosz Pieszko <br/>
bartosz.pieszko@gmail.com
