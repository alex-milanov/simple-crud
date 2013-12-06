<?php

	/**
	 *	record.inc
	 *
	 *	Active Record Implementation	
	 *	second layer database abstraction -> uses the db static class
	 *
	 */

	class record 
	{
		private $table;
		private $primary_key;
		private $data = array();
		
		function __construct($table, $primary_key)
		{
			$this->table = $table;
			$this->primary_key = $primary_key;
		}
		
		function set($field, $value)
		{
			$this->data[$field] = $value;
		}
		
		function get($field,$default='')	
		{
			return (isset($this->data[$field])) ? $this->data[$field] : $default;
                        
		}
		
		function get_data()
		{
			return $data;
		}
		
		function read($field, $value)	
		{
			$fields = array();
			
			$cond = array(array($field, $value));
			
			$records = db::select($this->table, $fields, $cond);
			
			if(is_array($records) && !empty($records) && !empty($records[0])){
				$this->data = $records[0];
				return true;
			}
			
			return false;
		}
		
		function read_pk($value)	
		{
			return $this->read($this->primary_key, $value);
		}
		
		function insert()	
		{
			$data = $this->data;
			
			return db::insert($this->table,$data);
		}
		
		function update()	
		{
			if(!isset($this->data[$this->primary_key])){
				return $this->insert();
			}
			
			$data = $this->data;
			
			$cond = array(array($this->primary_key, $data[$this->primary_key]));
			
			unset($data[$this->primary_key]);
			
			return db::update($this->table, $data, $cond);
		}
		
		function reset()
		{
			$this->data = array();
		}
		
	}
	

?>