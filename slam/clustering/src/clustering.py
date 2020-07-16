import rospy
import numpy as np
import sys
import random
import math
from sklearn.neighbors import KDTree
import sensor_msgs.point_cloud2 as pc2
from clustering.msg import Points



class Clustering:
    def __init__(self):
        self._pub_2d_obstacle_points = rospy.Publisher("/2d_obstacle_points", Points, queue_size=1)
        self._pub_3d_obstacle_points = rospy.Publisher("/3d_obstacle_points", Points, queue_size=1)
        self._sub = rospy.Subscriber("/velodyne_points", self.callback_point_clouds)
        self._iteration = 100
        self._remove_tolerance = 10.0
        self._plane_tolerance = 0.05
        self._clustering_tolerance = 0.01
        
    
    def projection(point, plane_config):
        a = plane_config['a']
        b = plane_config['b']
        c = plane_config['c']
        d = plane_config['d']
        t = (point[0]*a + point[1]*b + point[2]*c + d)/(a*a+b*b+c*c)
        return np.array([point[0]-a*t, point[1]-b*t, point[2]-c*t]).reshape(-1,1)


    def point_norm(point):
        return math.sqrt(point[0]*point[0] + point[1]*point[1] + point[2]*point[2])


    def load_point_clouds(self, msg, distance_tolerance):
        cloud_points = []
        cloud_channels = []
        for point in pc2.read_points(msg, field_names={"x","y","z","intensity","ring"}, skip_nans=True):
            if point[0] > 0 and math.sqrt(point[0]*point[0]+point[1]*point[1]+point[2]*point[2]) < distance_tolerance:
                cloud_points.append([point[0],point[1],point[2]])
                cloud_channels.append(point[4])
        return cloud_points, cloud_channels


    def ransac_plane(self, points, iteration, distance_tolerance):
        inliersResult = {}
        inliersResult = set()
        plane_config = {'a':0.0, 'b':0.0, 'c':0.0, 'd':0.0}
        while iteration:
            inliers = {}
            inliers = set()
            while len(inliers) <3:
                inliear.add(random.randint(0,len(points)-1))
            inliers_iter = inliers.__iter__()
            itr = next(inliers_iter)
            x1 = points[itr][0]
            y1 = points[itr][1]
            z1 = points[itr][2]
            itr = next(inliers_iter)
            x2 = points[itr][0]
            y2 = points[itr][1]
            z2 = points[itr][2]
            itr = next(inliers_iter)
            x3 = points[itr][0]
            y3 = points[itr][1]
            z3 = points[itr][2]
            # Get Normal Vector by Cross Product
            a = ((y2-y1)*(z3-z1) - (y3-y1)*(z2-z1))
            b = ((z2-z1)*(x3-x1) - (x2-x1)*(z3-z1))
            c = ((x2-x1)*(y3-y1) - (x3-x1)*(y2-y1))
            d = -(a*x1 + b*y1 + c*z1)
            for i in range(len(points)):
                # Not consider three points already picked
                if i in inliers:
                    continue
            x4 = points[i][0]
            y4 = points[i][1]
            z4 = points[i][2]
            # Distance between picked point and the plane
            dist = math.fabs(a*x4 + b*y4 + c*z4 +d) / math.sqrt(a*a + b*b + c*c)
            if dist <= distanceTol:
                inliers.add(i)
        if len(inliers) > len(inliersResult):
            inliersResult = inliers
            plane_config['a'] = a
            plane_config['b'] = b
            plane_config['c'] = c
            plane_config['d'] = d
        iteration -= 1
    return inliersResult, plane_config

    def filtering_points(self, points, inliers):

    
    def projecting_points(self, points, plane_config):
        lidar_position = self.projection([0,0,0],plane_config)
        lidar_x = self.projection([1,0,0],plane_config)
        lidar_y = self.projection([0,1,0],plane_config)
        lidar_x = lidar_x / self.point_norm(lidar_x)
        lidar_y = lidar_y / self.point_norm(lidar_y)
        L = np.hstack((lidar_x, lidar_y)) # lidar matrix, we will calculate (LtL)^-1Lt
        L = np.matmul(np.linalg.inv(np.matmul(np.transpose(L),L)),np.transpose(L))
        return np.matmul(L,np.transpose(np.array(points)))


    def euclidean_clustering(self, points, tree, distance_tolerance):


    def callback_point_clouds(self, msg):
        cloud_points, cloud_channels = self.load_point_clouds(msg, self._remove_tolerance)
        inliers, plane_config = self.ransac_plane(cloud_points, self._iteration, self._plane_tolerance)
        filtered_points, filtered_channels = self.filtering_points(cloud_points, inliers)
        projected_points = self.projecting_points(filtered_points, plane_config)
        tree = KDTree(projected_points)
        clusters = self.euclidean_clustering(projected_points, tree, self._clustering_tolerance)
        publish_clusters = []
        publish_channels = []
        publish_points = []
        publish_projected_points = []
        for cluster in clusters:
            for point in cluster:

        publish_2d_msg = Points()
        publish_3d_msg = Points()
        publish_2d_msg.is_3d = False
        publish_3d_msg.is_2d = True




if __name__ == '__main__':
    rospy.init_node("clustering")
    clustering = Clustering()
    rospy.spin()