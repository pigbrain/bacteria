# bacteria
bacteria is a debugger for java program which is running in service.  
bacteria is composed of plasmid and microscope.  
  
### plasmid  
Plasmid is agent library written with jvmti(JVM Tool Interface).  
If you want to use this, when you run your java application, you have to set up option `-agentlib`.
Plasmid is developed using gcc-4.1.2 on CentOS.  
  
You can compile with just `make` command.
  
# How to run plasmid

```
java -agentlib:plasmid={log path} -cp . Test  
  
java -agentlib:plasmid=./logs -cp . Test  
```
  
### microscopse  
Microscope is a spring boot application for viewing logs which were written by plasmid.  
If you want to change web server port, you have to modify `server.port` value of `src/main/resources/applicatoin.properties`.
also If you want to change log path, you have to modify `bacteria.plasmid.log.path` value of `src/main/resources/applicatoin.properties`.
  
  

  
