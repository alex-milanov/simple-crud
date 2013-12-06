<?php

	/**
	 *	parser.inc
	 *
	 *	Basic Template Parser Class	
	 *	opens .tpl files that can contain text, html, csv...
	 *
	 */

	class parser 
	{
		private $data = array();
		private $key_format = "$[val]";
		private $content = '';
		
		function __construct($file_name, $key_format="")
		{
			if(file_exists($file_name)){
				$this->content = file_get_contents($file_name, true);
			}
			
			if($key_format != ""){
				$this->key_format = $key_format;
			}
		}
		
		public function assign ($key,$value)
		{
			$this->data[$key]=$value;
		}
		
		public function parse()
		{
			$output = $this->content;
			
			foreach ($this->data as $key => $value){	
				$formated_key = str_replace("val", $key, $this->key_format);
				$output = str_replace ($formated_key, $value, $output);
			}
			
			return $output;
		}
		
		public function reset()
		{
			$this->data = array();
		}
	}
	

?>