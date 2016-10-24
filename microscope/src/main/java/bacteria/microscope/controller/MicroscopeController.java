package bacteria.microscope.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import bacteria.microscope.service.MicroscopeService;

@Controller
public class MicroscopeController {

	@Autowired
	private MicroscopeService microscopeService;
	
	
	@RequestMapping(value = "/", method = RequestMethod.GET)
	public String microscope(Model model) {

		model.addAttribute("exceptionLogList", microscopeService.getExceptionLogList());
		
		return "microscope";
	}
	
	@RequestMapping(value = "/{key}", method = RequestMethod.GET)
	public String viewLog(@PathVariable("key")Long key, Model model) {

		model.addAttribute("exceptionLogList", microscopeService.getExceptionLogList());
		model.addAttribute("exceptionLog", microscopeService.getExceptionLog(key));
		
		return "microscope";
	}
}
