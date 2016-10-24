package bacteria.microscope.service;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchEvent.Kind;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import bacteria.microscope.model.ExceptionLog;

@Service
public class LogWatchService {

	private Logger logger = LoggerFactory.getLogger(LogWatchService.class);

	@Value("${bacteria.plasmid.log.path}")
	private String logPath;

	private WatchService watchService;
	
	private List<ExceptionLog> logList;
	
	private Map<Long, String> logMap;

	@PostConstruct
	public void init() {
		try {
			logList = Collections.synchronizedList(new ArrayList<>());
			
			logMap = new ConcurrentHashMap<>();
			
			File logDirectory = new File(logPath); 
			for(File logFile : logDirectory.listFiles()) {
				logList.add(ExceptionLog.of(logFile.toPath().toString()));
			}
			
			watchService = FileSystems.getDefault().newWatchService();

			Path folder = Paths.get(logPath);

			folder.register(watchService, StandardWatchEventKinds.ENTRY_CREATE);
			
		} catch (IOException e) {
			logger.error("Init watchService instance is failed", e);
		}
	}

	@PreDestroy
	public void destroy() {
		try {
			watchService.close();
		} catch (IOException e) {
			logger.error("destroy watchService instance is failed", e);
		}
	}

	@Scheduled(fixedRate = 3000)
	private void processPage() {
		
		WatchKey watchKey = watchService.poll();
		if (watchKey == null) {
			return;
		}
		
		for (WatchEvent<?> watchEvent : watchKey.pollEvents()) {
			final Kind<?> eventKind = watchEvent.kind();

			if (StandardWatchEventKinds.OVERFLOW == eventKind) {
				continue;
			}

			if (StandardWatchEventKinds.ENTRY_CREATE == eventKind) {
				
				logger.debug("add element into logList[" + watchEvent.context() + "]");
				 
				logList.add(ExceptionLog.of(logPath + "/" + ((Path) watchEvent.context()).toString()));
			}
		}
		
		watchKey.reset();
	}
	
	public List<ExceptionLog> getExceptionLogList() {
		return new ArrayList<>(logList);
	}
	
	public String getExceptionLog(long key) {
		
		return logMap.getOrDefault(key, readLog(key));
	}
	
	public String readLog(long key) {
		Optional<ExceptionLog> exceptoinLog = logList.stream().filter((ExceptionLog x)-> x.getKey() == key).findAny();
		if (!exceptoinLog.isPresent()) {
			return StringUtils.EMPTY;
		}
		
		try {
			String contents = new String(Files.readAllBytes(Paths.get(exceptoinLog.get().getFilePath())));
			return contents;
		} catch (IOException e) {
		}
		
		return StringUtils.EMPTY;
	}
}
