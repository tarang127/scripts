<?php 
class node {
   // Private list of links/nodes for each node.
   private $links;
 
   // Count of number of strings this node carries.
   private $count;

   // List of RETURN_COUNT number of strings stored
   // for each node, this is returned when we match 
   // and ask for matches.
   // TODO: Update this to make it smarter. It is 
   // alphabetical for now.
   private $matches;

   // Constructor to initialize array type
   function __construct() {
       $this->links = array();
       $this->matches = array();
       $this->count = 0;
   }

   // Get all the links, if at least one of them is defined.
   public function getLinks() {
        if(!empty($this->links)) {
                return $this->links;
        }
        else return null;
   }

   // Add an entry to the node tree (with the links, if it doesn't
   // exist already.
   public function addEntry($char) {
	if(!empty($this->links) && array_key_exists($char, $this->links)) {
                return null;
        }
        else {
                $this->links[$char] = new node;
                return $this->links[$char];
        }
   }

   // Return the node for a char if it exists.
   public function getNodeAt($char) {
        if(!empty($this->links) && array_key_exists($char, $this->links)) {
                return $this->links[$char];
        }
        else return null;
   }

   // Update the list and the count for this node
   public function updateList($input) {
	$this->count = ($this->count < 5) ? $this->count + 1 : 5;
	array_push($this->matches, $input);
	sort($this->matches);
	//var_dump($this->matches);
	$this->matches = array_slice($this->matches, 0, 5);	
   }

   public function getList() {
	return $this->matches;
   }
}

class TRIE {

   // Root of the TRIE
   private $ROOT = null;

   // TERM_CONST to indicate termination of every string entry.
   const TERM_CONST= '?';

   // Constructor to initialize $ROOT type
   function __construct() {
   		$this->ROOT = new node();
   }
   

   public function printTrie() {
        // This is basically BFS
        /*$nodelist = array();
        array_push($nodelist, $this->ROOT);
        while(!empty($nodelist)) {
                $currnode = array_shift($nodelist);
		$currlist = $currnode->getLinks();
		if(!empty($currlist)) {
                   foreach ($currlist as $key => $value) {
                     echo "$key\n"; var_dump($value);
                     array_push($nodelist, $value);
                   }
                }
        }*/
//	var_dump($this->ROOT);
   }

   public function addWord($input) {
	$myFile = "/home5/investa5/public_html/error_log";
	$fh = fopen($myFile, 'a+');
	$input = strtoupper($input);
        $temp = $input;

	// Append special char to the end of the string to make it unique
	// So no input string is a pure prefix of any existing string.
	$temp .= self::TERM_CONST; 
	//echo $temp;
        // Convert the string to array so we can traverse each char
        $word = str_split($temp);
        $currnode = $this->ROOT;
        $index = -1;

        // For each character in the input string, check if it
        // exists in the tree, if not add it.
        foreach ($word as &$char) {
	   $index++;
	   if (($nextnode = $currnode->getNodeAt($char))!=null) {
             //echo "$char is found\n";
	     $currnode = $nextnode;
           }
           else {
             //echo "$char doesn't exist\n";
		if(($newnode = $currnode->addEntry($char))!=null) {
		$currnode = &$newnode;
		//echo "Added $char\n";
	     }
	     else {
		return false;
		//echo "This should never happen\n";
	     }
           }
	   // Add word to the list and update count. 
	   if ($char != self::TERM_CONST) {
		//$pattern = substr($input, $index);
		$currnode->updateList($input);
	   }
        }
	fclose($fh);
	return true;
   }

   public function exists($input) {
	$input = strtoupper($input);

	// Append special char to the end of the string to make it unique
        // So no input string is a pure prefix of any existing string.
        //$input .= self::TERM_CONST;

	// Convert the string to array so we can traverse each char
        $word = str_split($input);	

	// Start from root until the word ends and we reach TERM_CHAR;
	$currnode = $this->ROOT;

	// For each character in the input string, check if it
        // exists in the tree, if not return null and let calling
	// module handle it.
	foreach ($word as &$char) {
	   if (($nextnode = $currnode->getNodeAt($char))!=null) {
		//echo "Found $char, looking for next now\n";
		$currnode = $nextnode;
	   }
	   else {
		//echo "Didn't find $input, search terminated at $char\n";
		return false;
	   }
	}
	return true;
   }

   public function getMatchingWords($input) {
	$startTime = microtime(true);

	$input = strtoupper($input);
	//$input .= self::TERM_CONST;
	//$fh = fopen("/home5/investa5/logFile2.txt", 'a+');
	
        // Convert the string to array so we can traverse each char
        $word = str_split($input);

        // Don't append TERM_CONST to input because we want to find all matching words.
	// Start from root until the word ends.
	$currnode = $this->ROOT;
        	
	// For each character in the input string, check if it
        // exists in the tree. If less than 3 chars match
	// return null.
	$countMatch = 0;
	foreach ($word as &$char) {
	   //fwrite($fh, "Looking for char $char\n");
	   /*if ($char == self::TERM_CONST) {
		break;
	   }*/
	   if (($nextnode = $currnode->getNodeAt($char))!=null) {
                //fwrite($fh, ">> Found $char, looking for next now\n");
                $currnode = $nextnode;
		$countMatch++;
           }
           else {
                //fwrite($fh, ">>> Didn't find $input, search terminated at $char\n");
		if ($countMatch < 3) return null;
		else break;
           }
        }
	$endTime = microtime(true);
	$elapsed = $endTime - $startTime;
	//fwrite($fh, "Time taken : ".$elapsed." seconds\n");
        //fclose($fh);
	return $currnode->getList();
   }

   public function parseFileBuildTrie($filename) {
	//$fh = fopen("/home5/investa5/public_html/error_log", 'a+');
	$handle = @fopen($filename, "r");
        if ($handle) {
    		//fwrite($fh, "Fopen was successful\n");
		while (($buffer = fgets($handle)) !== false) {
			//fwrite($fh, "Adding word $buffer\n");
			$this->addWord(trim($buffer));
    		}
    		if (!feof($handle)) {
        		//fwrite("$fh, Error: unexpected fgets() fail\n");
    		}
    		//fclose($handle);
	}
	//else fwrite($fh, "fopen failed..what to do?\n");
	//fclose($fh);
   }
}
?>
