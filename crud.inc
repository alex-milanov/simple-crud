<?php

	/**
	 *	crud.inc
	 *
	 *	CRUD Pattern Implementation	
	 *
	 */

	class crud
	{
                private $table;
                private $primary_key;
                private $fields = array();
                private $errors = array();
                
                
                public function __construct($table,$primary_key,$fields) 
                {
                    $this->table = $table;
                    $this->primary_key = $primary_key;
                    $this->fields = $fields;
                }
                
                function list_records()
                {
                    $list_parser = new parser(TPL_PATH.'/'.$this->table.'/list.tpl');
                    $list_row_parser = new parser(TPL_PATH.'/'.$this->table.'/list_row.tpl');

                    $records = array();
                    $output_html = ''; 

                    // get records
                    $records = db::select($this->table);

                    // assign data
                    $rows_html = '';
                    foreach ($records as $record){
                        $list_row_parser->reset();
                        $list_row_parser->assign($this->primary_key, $record[$this->primary_key]);
                        foreach($this->fields as $field){
                            $list_row_parser->assign($field, $record[$field]);
                        }
                        
                        $rows_html .= $list_row_parser->parse();
                    }

                    $list_parser ->assign('rows', $rows_html);

                    // parse
                    $output_html = $list_parser->parse();

                    return  $output_html;
                }
                
                function validate()
                {
//                    $errors

                    return true;       
                }
            
                function edit()
                {
                    $form_parser = new parser(TPL_PATH.'/'.$this->table.'/form.tpl');

                    $record = new record($this->table,$this->primary_key);
                    $output_html = ''; 
                    $id = 0;  
                    // get record if editing
                    if(isset($_REQUEST[$this->primary_key]) && $_REQUEST[$this->primary_key]>0){
                        $id = $_REQUEST[$this->primary_key];
                        $record->read_pk($id);
                    } 

                    //assing errors
                    foreach($this->fields as $field){
                        if(isset($this->errors[$field])){
                            $form_parser->assign($field.'_error',$this->errors[$field]);
                        } else {
                            $form_parser->assign($field.'_error','');
                        }
                    }

                    // assign data
                    $form_parser->assign($this->primary_key, $id);
                    
                    foreach($this->fields as $field){
                        if(isset($_REQUEST[$field])){
                            $form_parser->assign($field,$_REQUEST[$field]);
                        } else {
                            $form_parser->assign($field,$record->get($field));
                        }
                    }
                    
                    // parse
                    $output_html = $form_parser->parse();

                    return  $output_html;
                    
                }
                
                function save()
                {
                    if(!$this->validate()){
                        return $this->edit();
                    } 
                    
                    $record = new record($this->table,$this->primary_key);

                    // check for "id"
                    if(isset($_REQUEST[$this->primary_key]) && $_REQUEST[$this->primary_key]>0){
                        $record->set($this->primary_key, $_REQUEST[$this->primary_key]);
                        // promenliva za id 
                    }

                    // set the fields
                    foreach($this->fields as $field){
                        $record->set($field, $_REQUEST[$field]);
                    }
                    
                    // save record
                    $record->update();

                    return $this->list_records();
                }
		
                function delete()
                {
                    
                }
                
                
	}


?>