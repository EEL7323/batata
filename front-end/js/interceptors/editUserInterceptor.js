angular.module("ruServer").factory("editUserInterceptor", function ($q, $rootScope, $timeout, $location) {
    return {
        request: function (config) {
            if (config.url.startsWith("../back-end/php/insertUser.php?") || config.url.startsWith("../back-end/php/updateUser.php?")) {
                $rootScope.checkUrlManage = true;
            }
            return config;
        },
        requestError: function (rejection) {
            $rootScope.checkUrlManage = false;
            return $q.reject(rejection);
        },
        response: function (response) {
            if ($rootScope.checkUrlManage) {
                $timeout(function () {
                    $rootScope.checkUrlManage = false;
                    $location.path("/admin/manage-users");
                }, 1500);
            }
            return response;
        },
        responseError: function (rejection) {
            $rootScope.checkUrlManage = false;
            return $q.reject(rejection);
        }
    };
});
