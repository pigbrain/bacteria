package bacteria.microscope.service;

import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import bacteria.microscope.model.ExceptionLog;

@Service
public class MicroscopeService {

	@Autowired
	private LogWatchService logWatchService;
	
	
	public List<ExceptionLog> getExceptionLogList() {
		
		return logWatchService.getExceptionLogList();
	}
	
	public String getExceptionLog(Long key) {
		if (key == null) {
			return StringUtils.EMPTY;
		}
		
		return logWatchService.getExceptionLog(key);
	}
}
