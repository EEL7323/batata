angular.module("ruServer").controller("adminEditUserCtrl", function ($scope, $http, $location, config, $rootScope) {

    $scope.user = $rootScope.selUser;
    $scope.editUserTitle = $rootScope.editUserTitle;

    $scope.getMethod = function () {
        if ($rootScope.editUserTitle == "Insert user info") return "insert";
        else if ($scope.editUserTitle == "Edit user info") return "update";
    }

    $scope.saveUser = function (user) {
        var _token = localStorage['ruServer'];
        _sourceAdmin = _token.slice(19);
        var _postData = {
            tagNumber: user.tagNumber,
            sourceRegistryNumber: _sourceAdmin,
            targetRegistryNumber: user.registryNumber,
            name: user.name,
            accessLevel: user.accessLevel
        };

        if ($scope.getMethod() == 'insert') {
            $http.post(config.serverBaseUrl + "insertUser.php", _postData)
                .then(function (response) {
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                });
        }
        else if ($scope.getMethod() == 'update') {
            $http.post(config.serverBaseUrl + "updateUser.php", _postData)
                .then(function (response) {
                    console.log(response.data);
                })
                .catch(function (error) {
                    console.log(error);
                });
        }
        $location.path("/admin/manage-users");
    };
});