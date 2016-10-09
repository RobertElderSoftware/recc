<style type="text/css">
/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/
Typedefed typenames
bitfield or declarater aggregation
scoped and unscoped enums

*/

$type_engine_things = Array(
	Array(
		"name" => "aggregate_specifier",
		"details" => Array(
			"name" => "struct aggregate_specifier_details",
			"members" => Array(
				"enum type_engine_aggregate_specifier_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_SIMPLE_SPECIFIER",
			"TYPE_ENGINE_STRUCT_OR_UNION_SPECIFIER",
			"TYPE_ENGINE_ENUM_SPECIFIER",
			"TYPE_ENGINE_TYPENAME_SPECIFIER"
		)
	),
	Array(
		"name" => "simple_type_specifier",
		"details" => Array(
			"name" => "enum simple_type_specifier_kind",
			"nodetails" => true,
			"members" => Array()
		),
		"types" => Array("VOID", "CHAR", "SHORT", "INT", "LONG", "FLOAT", "DOUBLE", "SIGNED", "UNSIGNED")
	),
	Array(
		"name" => "simple_type_qualifier",
		"details" => Array(
			"name" => "enum simple_type_qualifier_kind",
			"nodetails" => true,
			"members" => Array()
		),
		"types" => Array("CONST", "VOLATILE")
	),
	Array(
		"name" => "simple_storage_class_specifier",
		"details" => Array(
			"name" => "enum simple_storage_class_specifier_details",
			"nodetails" => true,
			"members" => Array()
		),
		"types" => Array("TYPEDEF", "EXTERN", "STATIC", "AUTO", "REGISTER")
	),
	Array(
		"name" => "specifier_or_qualifier",
		"details" => Array(
			"name" => "struct specifier_or_qualifier_details",
			"members" => Array(
				"enum type_engine_specifier_or_qualifier_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_SIMPLE_STORAGE_CLASS_SPECIFIER",
			"TYPE_ENGINE_SIMPLE_QUALIFIER",
			"TYPE_ENGINE_AGGREGATE_TYPE_SPECIFIER"
		)
	),
	Array(
		"name" => "specifier_or_qualifier_list_item",
		"details" => Array(
			"name" => "struct specifier_or_qualifier_list_item_details",
			"members" => Array(
				"enum type_engine_specifier_or_qualifier_list_item_kind type",
				"struct specifier_or_qualifier_id item_id",
				"struct specifier_or_qualifier_list_item_id prev_id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER",
			"TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START"
		)
	),
	Array(
		"name" => "parameter_list",
		"details" => Array(
			"name" => "struct parameter_list_details",
			"members" => Array(
				"enum type_engine_parameter_list_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_PARAMETER_LIST_VARIADIC",
			"TYPE_ENGINE_PARAMETER_LIST_NON_VARIADIC"
		)
	),
	Array(
		"name" => "declarator_part",
		"details" => Array(
			"name" => "struct declarator_part_details",
			"members" => Array(
				"enum type_engine_declarator_part_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
        		"TYPE_ENGINE_FUNCTION_DECLARATOR_PART",
        		"TYPE_ENGINE_POINTER_DECLARATOR_PART",
		        "TYPE_ENGINE_ARRAY_DECLARATOR_PART"
		)
	),
	Array(
		"name" => "declarator_part_list_item",
		"details" => Array(
			"name" => "struct declarator_part_list_item_details",
			"members" => Array(
				"enum type_engine_declarator_part_list_item_kind type",
				"struct declarator_part_id item_id",
				"struct declarator_part_list_item_id prev_id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART",
			"TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START"
		)
	),
	Array(
		"name" => "pointer_part",
		"details" => Array(
			"name" => "struct pointer_part_details",
			"members" => Array(
				"enum type_engine_pointer_part_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_QUALIFIED_POINTER",
			"TYPE_ENGINE_UNQUALIFIED_POINTER"
		)
	),
	Array(
		"name" => "function_part",
		"details" => Array(
			"name" => "struct function_part_details",
			"members" => Array(
				"enum type_engine_function_part_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_FUNCTION_K_AND_R_C",
			"TYPE_ENGINE_FUNCTION_PROTOTYPE"
		)
	),
	Array(
		"name" => "array_part",
		"details" => Array(
			"name" => "struct array_part_details",
			"members" => Array(
				"enum type_engine_array_part_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY",
			"TYPE_ENGINE_FLEXIBLE_ARRAY"
		)
	),
	Array(
		"name" => "bitfield_or_declarator",
		"details" => Array(
			"name" => "struct bitfield_or_declarator_details",
			"members" => Array(
				"enum type_engine_bitfield_or_declarator_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_HAS_BITFIELD",
			"TYPE_ENGINE_DOES_NOT_HAVE_BITFIELD"
		)
	),
	Array(
		"name" => "general_type",
		"details" => Array(
			"name" => "struct general_type_details",
			"members" => Array(
				"struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id",
				"struct bitfield_or_declarator_id bitfield_or_declarator_id"
			)
		),
		"types" => Array(
		)
	),
	Array(
		"name" => "general_type_list_item",
		"details" => Array(
			"name" => "struct general_type_list_item_details",
			"members" => Array(
				"enum type_engine_general_type_list_item_kind type",
				"struct general_type_id item_id",
				"struct general_type_list_item_id prev_id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE",
			"TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_LIST_START"
		)
	),
	Array(
		"name" => "struct_or_union_specifier",
		"details" => Array(
			"name" => "struct struct_or_union_specifier_details",
			"members" => Array(
				"enum type_engine_struct_or_union_specifier_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_STRUCT_SPECIFIER",
			"TYPE_ENGINE_UNION_SPECIFIER"
		)
	),
	Array(
		"name" => "scoped_struct_or_union_specifier",
		"details" => Array(
			"name" => "struct scoped_struct_or_union_specifier_details",
			"members" => Array(
				"struct scope_level_id scope_id",
				"struct struct_or_union_specifier_id struct_or_union_specifier_id"
			)
		),
		"types" => Array(
		)
	),
	Array(
		"name" => "union_specifier",
		"details" => Array(
			"name" => "struct union_specifier_details",
			"members" => Array(
				"enum type_engine_union_specifier_kind type",
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_ANONYMOUS_UNION",
			"TYPE_ENGINE_NAMED_UNION"
		)
	),
	Array(
		"name" => "struct_specifier",
		"details" => Array(
			"name" => "struct struct_specifier_details",
			"members" => Array(
				"enum type_engine_struct_specifier_kind type"
				"unsigned int id"
			)
		),
		"types" => Array(
			"TYPE_ENGINE_ANONYMOUS_STRUCT",
			"TYPE_ENGINE_NAMED_STRUCT"
		)
	),
	Array(
		"name" => "identifier",
		"details" => Array(
			"name" => "struct identifier_details",
			"members" => Array()
		),
		"types" => Array()
	),
	Array(
		"name" => "constant_expression_parser_node",
		"details" => Array(
			"name" => "struct constant_expression_parser_node_details",
			"members" => Array()
		),
		"types" => Array()
	),
	Array(
		"name" => "scope_level",
		"details" => Array(
			"name" => "struct scope_level_details",
			"members" => Array()
		),
		"types" => Array()
	)
);

echo '<div class="type-engine-table">';
echo "<h1>Type Engine</h1>";

foreach($type_engine_things as $thing){
	echo '<div class="thing-group">';
	echo "<h1>{$thing["name"]}</h1>";
	echo "enum {$thing["name"]}_kind{<br/>";
	for($i = 0; $i < count($thing["types"]); $i++){
		$type = $thing["types"][$i];
		echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{$type}";
		if($i != (count($thing["types"]) -1)){
			echo ",";
		}
		echo "<br/>";
	}
	echo "}<br/>";
	echo "<br/>";
	if(!isset($thing["details"]["nodetails"])){
		echo "{$thing["details"]["name"]}{<br/>";
		for($i = 0; $i < count($thing["details"]["members"]); $i++){
			$member = $thing["details"]["members"][$i];
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{$member}";
			if($i != (count($thing["details"]["members"]) -1)){
				echo ",";
			}
			echo "<br/>";
		}
		echo "}<br/>";
	}
	echo "<h3>List of '{$thing["details"]["name"]}'</h3>";
	echo "<h3>Map of '{$thing["details"]["name"]}' -> 'struct {$thing["name"]}_id'</h3>";
	echo "</div>";
}

echo "</div>";

?>
The sha1sum of the lines above is ca9374cd0bb7a418d7d917cf21de0f147452cb72
