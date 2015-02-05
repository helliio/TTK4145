{-# LANGUAGE OverloadedStrings #-}

import Network.Simple.TCP
import qualified Data.ByteString.Char8 as C

main = connect "127.0.0.1" "34933" $ \(socket, remoteAddr) -> do
   putStrLn $ "Connection established to " ++ show remoteAddr
   send socket (C.pack message)

message = take 1024 $ cycle "lorem ipsum "
