#include <iostream>
#include <vector>

std::vector<int> line(int start_x, int start_y, int end_x, int end_y){
	
	int dy = (end_y - start_y);
	int dx = (end_x - start_x);
	
	//std::vector<std::vector<float>> coords;
	std::vector<int> coords;
	
	// L2R
	// 0<m<1
	if((dy <= dx)&&(dy*dx >= 0)){
		int d_start = 2*dy - dx;
		int d_NE = 2*(dy - dx);
		int d_E = 2*dy;
		
		int x = start_x;
		int y = start_y;
		int d = d_start;
		
		while(x < end_x){
		
			std::cout<<x<<", "<<y<<std::endl;
			std::vector<int> new_coords{x, y, 0};
			coords.insert(coords.end(), new_coords.begin(), new_coords.end());
			
			if(d >= 0){
				y += 1;
				d += d_NE;
			} else if(d < 0){ d+= d_E; }
			
			x += 1;
		}
	} else if((dy >= dx)&&(dy*dx >= 0)){         // fwd m>1
		int d_start = 2*dx - dy;
		int d_NE = 2*(dx - dy);
		int d_N = 2*dx;
		
		int x = start_x;
		int y = start_y;
		int d = d_start;
		
		while(y < end_y){
			
			std::cout<<x<<", "<<y<<std::endl;
			std::vector<int> new_coords{x, y, 0};
			coords.insert(coords.end(), new_coords.begin(), new_coords.end());
			
			if(d >= 0){
				x += 1;
				d += d_NE;
			} else if(d < 0){ d += d_N; }
			
			y += 1;
		}
	} else if((-dy <= dx)&&(dy*dx <= 0)){                         // fwd -1<m<0
		int d_start = -dx - 2*dy;
		int d_E = -2*dy;
		int d_SE = -2*(dx + dy);
		
		int x = start_x;
		int y = start_y;
		int d = d_start;
		
		while(x < end_x){

			std::cout<<x<<", "<<y<<std::endl;
			std::vector<int> new_coords{x, y, 0};
			coords.insert(coords.end(), new_coords.begin(), new_coords.end());
			
			if(d >= 0){
				y -= 1;
				d += d_SE;
			} else if(d < 0){ d += d_E; }
			
			x += 1;
		}
	} else if((-dy >= dx)&&(dy*dx <= 0)){
		
		int d_start = dy + 2*dx;
		int d_S = 2*dx;
		int d_SE = 2*(dy + dx);
		
		int x = start_x;
		int y = start_y;
		int d = d_start;
		
		while(y > end_y){
			
			if(d >= 0){ 
				d += d_SE;
				x += 1;
			}
			else if(d < 0){	d += d_S; }
			
			y -= 1;
			
			std::cout<<x<<", "<<y<<std::endl;
			std::vector<int> new_coords{x, y, 0};
			coords.insert(coords.end(), new_coords.begin(), new_coords.end());
		}
	}
	
	for(int j = 0; j < coords.size(); j++){
		std::cout<<coords[j]<<" ";
	}
	
	return coords;
}
