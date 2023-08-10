#!/bin/bash

delcare -A employees

function emp_add(){
	echo "Enter Employee Name:"
    read name
    echo "Enter Employee Number:"
    read emp_number
    echo "Enter Telephone Number:"
    read phone_number

    employees["$emp_number"]="$name,$phone_number"
    echo "Employee Record added successfully!"
}

function emp_del(){
	echo "Enter Employee No -- "
    read emp_number

    if [[ ${employees["$emp_number"]} ]]; then
        unset employees["$emp_number"]
        echo "Employee Record deleted successfully!"
    else
        echo "Employee Number not found in the record!"
    fi
	
}

function emp_search(){
	echo "Enter Employee No -- "
	read emp_number

    if [[ ${employees["$emp_number"]} ]]; then
        echo "Employee Name: ${employees["$emp_number"]%,*}"
        echo "Telephone Number: ${employees["$emp_number"]#*,}"
    else
        echo "Employee Number not found in the record!"
    fi
}

function emp_sort(){
	echo "Sorting Employee -- "
	if [[ ${#employees[@]} -eq 0 ]]; then
        echo "Employee Record is empty!"
    else
        echo "Sorted Employee Records:"
        for emp_number in $(echo "${!employees[@]}" | tr ' ' '\n' | sort); do
            echo "Employee Number: $emp_number"
            echo "Employee Name: ${employees["$emp_number"]%,*}"
            echo "Telephone Number: ${employees["$emp_number"]#*,}"
            echo "-------------------------"
        done
    fi
}

function emp_list(){
	echo "Listing Employee -- "
	if [[ ${#employees[@]} -eq 0 ]]; then
        echo "Employee Record is empty!"
    else
        echo "Employee Records:"
        for emp_number in "${!employees[@]}"; do
            echo "Employee Number: $emp_number"
            echo "Employee Name: ${employees["$emp_number"]%,*}"
            echo "Telephone Number: ${employees["$emp_number"]#*,}"
        done
    fi
}

while true
do
	echo "Welcome to the Employee Desk - "
	echo "Menu"
	echo "(1) -> Create Employee Record"
	echo "(2) -> Delete Employee Record"
	echo "(3) -> Search Employee Record"
	echo "(4) -> Sort Employee Record"
	echo "(5) -> List all Employee Record"
	echo "(6) -> Exit"
	echo "Enter your choice (1 - 7) : "
	read choice

	case $choice in 
		(1)
			emp_add
			;;
		(2)
			emp_del
			;;
		(3)
			emp_search
			;;
		(4)
			emp_sort
			;;
		(5)
			emp_list
			;;
		(6	)
			echo "Thank You..."
			break
			;;
		(*)
			echo "Please enter a valid command - "
			;;
	esac

done
