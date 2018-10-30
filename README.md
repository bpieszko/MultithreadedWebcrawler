# Multithreaded Webcrawler

## Description
Multithreaded webcrawler written in C++. It requires url of page to start crawling, regular expression of strings to search for, depth of recursion in which to look for strings defined by regular expression and number of threads to use.

## Compile
```
make
```

## Using
```
./app [url] [regex] [depth] [threads number]
    [url] - url of page to start crawling
    [regex] - regex of strings to search for
    [depth] - depth of crawling, 0 - crawl only in input page
    [threads number] - number of threads to use
```

## Example
```
./app.e 'https://panoramafirm.pl/szukaj?k=notariusz' '[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+' 2 4
```

## Output
```
139776489420544: kancelaria_notarialna@wp.pl
139776489420544: notariusz@flach-checinska.pl
139776489420544: a.buchaniewicz@izba.net.pl
139776489420544: notariusz.epz@gmail.com
139776489420544: kancelaria@notariusz-nowakowskasperka.pl
139776489420544: notariusz.kozek@gmail.com
139776489420544: rejent.czestochowa@gmail.com
139776638371584: danko@notariusze.waw.pl
139776638371584: zuczek@notariusze.waw.pl
139776638371584: azm-notariat-slupsk@wp.pl
139776638371584: zelichowski.notariusz@gmail.com
139776621586176: biuro.notamus@wp.pl
139776621586176: adw.eilmes@wp.pl
139776621586176: mateusz.w.kostka@gmail.com
139776621586176: adwokatilawa@gmail.com
139776621586176: notariusz@golub.com.pl
139776621586176: agata.ligas@kin.pl
139776621586176: awrzecionkowska@gmail.com
139776621586176: biuro@niemiecki-24.pl
139776621586176: agnieszka.baranska@gdin.pl
139776621586176: monika.sowinska@gdin.pl
139776621586176: genczelewska@notariusze.waw.pl
139776629978880: kancelaria@gomulkiewicz-krzywulicz.pl
139776629978880: kancelaria@notariusz-slupsk.pl
139776629978880: notariusz@kancelarianotarialnaslupsk.pl
139776629978880: sidor@notar.pl
139776629978880: katarzyna.wojdylo@notariuszligocka.pl
139776629978880: notariuszeraciborz@gmail.com
```

## Author
Bartosz Pieszko <br/>
bartosz.pieszko@gmail.com
