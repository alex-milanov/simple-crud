<?php

	/**
	 *	db.inc
	 *
	 *	Database Abstraction Static Class	
	 *	Implements the basic query functions with some special features
	 *
	 */

	class db 
	{

		// singleton insures that the connection is created only once
		static function get_connection()
		{
			static $connection = NULL;
	
			if ($connection != NULL) {
				return $connection;
			}
			
			$connection = mysqli_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
			
			// Check connection
			if (mysqli_connect_errno($connection)) {
				echo "Failed to connect to MySQL: " . mysqli_connect_error();
			}
	
			mysqli_set_charset($connection, "utf8");
			
			return $connection;
		}
	
		
		// abstraction of the mysqli_query function
		static function query($query) 
		{
			$connection = db::get_connection();
	
			$result = mysqli_query($connection, $query);
			
			/* here you can test the query */
			
			if ($result==false){
				echo $query."<br/>";
				echo ('Error: ' . mysqli_error($connection));
			}
			
			return $result;
		}
	
		static function prep_where($condition) 
		{
			if(is_string($condition)){
				return $condition;
			}
			
			if(!is_array($condition) || empty($condition)){
				return "1";
			}
			
			$where_arr = array();
			
			foreach ($condition as $cond) {
				
				$op = (isset($cond[2])) ? $cond[2] : '=';
				
				switch ($op) {
					case "=":
					case "!=":
					case "<":
					case ">":
					case "<=":
					case ">=":
					case "LIKE":
							$where_arr[] = "`$cond[0]` $op '$cond[1]'";
						break;
					
					case "FIND":
						$where_arr[] = "`$cond[0]` $op '%$cond[1]%'";
						break;
					
					case "IN":
						$where_arr[] = "`$cond[0]` $op ('".implode("', '",$cond[1])."')";
						break;
					
					default:
					
						break;
				}
			}
	
			if(empty($where_arr)){
				return "1";
			}
			
			$where_str = implode(" AND ", $where_arr);
			
			return $where_str;
		}
	
		static function select($table, $fields = array(), $cond = array()) {
	
			$records = array();

			if(empty($fields)){
				$fields_str = "*";
			} else {
				$fields_str = implode(", ", $fields);
			}
	
			$where_str = self::prep_where($cond);
			
			$query = "SELECT $fields_str FROM $table WHERE $where_str";
	
			$result = self::query($query);
			
			while ($record = mysqli_fetch_assoc($result)) {
				$records[] = $record;
			}
	
			return $records;
		}
		
		static function select_hash($table, $key_field, $value_field, $cond = array()) {
			
			$hash = array();
			
			$fields = array($key_field,$value_field);
			
			$records = self::select($table, $fields, $cond);
			
			foreach ($records as $record) {
				$hash[$record[$key_field]] = $record[$value_field];
			}
			
			return $hash;
		}
		
		
		static function prep_set($data)
		{
			$set_arr = array();
			
			foreach($data as $field_name => $field_value){
				$set_arr[] = "$field_name = '$field_value'";
			}
			
			$set_str = implode (", ",$set_arr);
			
			return $set_str;
		}
		
	
		static function insert($table, $data) 
		{
			$set_str = db::prep_set($data);
			
			$sql = "INSERT INTO $table SET $set_str";
			
			$result = db::query($sql);
	
			if ($result) {
				$connection = db::get_connection();
				$inserted_pk = mysqli_insert_id($connection);
				return $inserted_pk;
			}
			
			return false;
		}
	
		static function update($table, $data, $cond) {
			
			$set_str = db::prep_set($data);
			
			$where_str = db::prep_where($cond);
			
			$sql = "UPDATE $table SET $set_str WHERE $where_str";
			
			$result = db::query($sql);
			
			// check if the update is successful
			$connection = db::get_connection();
			$affected_rows = mysqli_affected_rows($connection);
			
			if ($affected_rows > 0) {
				return true;
			}
			return false;
		}
	
		static function delete($table, $cond) 
		{
			if (!isset($cond) || (is_array($cond) && empty($cond)) || $cond == "") {
				return false;
			}
			
			$where_str = db::prep_where($cond);
	
			$sql = "DELETE FROM $table WHERE $where_str";
			
			$result = self::query($sql);
	
			$connection = db::get_connection();
			$affected_rows = mysqli_affected_rows($connection);
			
			if ($affected_rows > 0) {
				return true;
			}
			
			return false;
		}
	
	}


?>