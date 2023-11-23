/** 
 * Describes city with a given name and unique integer identifier
 * @author Duncan, YOUR NAME
 * <pre>
 * Date: December 20, 2023
 * CSC 3102 Programming Project # 1
 * Instructor: Dr. Duncan 
 *
 * DO NOT REMOVE THIS NOTICE (GNU GPL V2):
 * Contact Information: duncan@csc.lsu.edu
 * Copyright (c) 2023 William E. Duncan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 * Note: DO NOT MODIFY THIS FILE
 * </pre>
 */


#ifndef CITY_H
#define	CITY_H
#include <string>
#include <iostream>

using namespace std;

class City
{
public:
    /**
     * Creates a default city
     */
   City()
   {
      label = " ";
      key = 0;
   }

   /**
    * Creates a city with the specified key (zip code) and label (name)
    * @param aKey key of the city
    * @param aLabel label of the city
    */
   City(int aKey, string aLabel)
   {
      label = aLabel;
      key = aKey;
   }

   /**
    * Creates an anonymous city with the specified key (zip code)
    * @param aKey the key of the city
    */
   City(int aKey)
   {
      label = " ";
      key = aKey;
   }

   /**
    * Gives the label for this city
    * @return the label for this city
    */
   string getLabel() const
   {
      return label;
   }

   /**
    * Gives the key for this city
    * @return the key for this city
    */
   int getKey() const
   {
      return key;
   }
   /**
    * Determines whether the specified cities are identical
    * @param c1 a city
    * @param c2 a city
    * @return true if the specified cities are identical;
    * false, otherwise
    */
   friend bool operator==(const City& c1, const City& c2); 
   /**
    * Determines whether the specified cities are different
    * @param c1 a city
    * @param c2 a city
    * @return true if the specified cities are different;
    * false, otherwise
    */   
   friend bool operator!=(const City& c1, const City& c2); 
   /**
    * Determines whether the key of the first city is greater than
    * the key of the second city.
    * @param c1 a city
    * @param c2 a city
    * @return true if the key of the first city is greater than the key
    * of the second city; false, otherwise
    */      
   friend bool operator>(const City& c1, const City& c2); 
   /**
    * Determines whether the key of the first city is less than
    * the key of the second city.
    * @param c1 a city
    * @param c2 a city
    * @return true if the key of the first city is less than the key
    * of the second city; false, otherwise
    */         
   friend bool operator<(const City& c1, const City& c2); 
   /**
    * Determines whether the key of the first city is greater than
    * or the same as the key of the second city.
    * @param c1 a city
    * @param c2 a city
    * @return true if the key of the first city is greater than or the same
    * as the key of the second city; false, otherwise
    */         
   friend bool operator>=(const City& c1, const City& c2); 
   /**
    * Determines whether the key of the first city is less than
    * or the same as the key of the second city.
    * @param c1 a city
    * @param c2 a city
    * @return true if the key of the first city is less than or the same
    * as the key of the second city; false, otherwise
    */       
   friend bool operator<=(const City& c1, const City& c2); 
private:
    /**
     * The label (name) of this city
     */
   string label;
   /**
    * The key (zip code) of this city    
    */
   int key;
};

bool operator==(const City& c1, const City& c2)
{
   return (c1.key == c2.key);
}

bool operator!=(const City& c1, const City& c2)
{
   return (c1.key != c2.key);
}

bool operator>(const City& c1, const City& c2)
{
   return (c1.key > c2.key);
}

bool operator<(const City& c1, const City& c2)
{
   return (c1.key < c2.key);
}

bool operator>=(const City& c1, const City& c2)
{
   return (c1.key >= c2.key);
}

bool operator<=(const City& c1, const City& c2)
{
   return (c1.key <= c2.key);
}
#endif	/* CITY_H */

